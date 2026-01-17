# redis with python tutorial :
# https://redis.io/docs/latest/develop/clients/redis-py/
#
# TODO: check about redis pipelining
import redis;
import psycopg2
import os
import requests
from urllib.parse import urlparse
from urllib.robotparser import RobotFileParser

def pop_url(r: redis.Redis):
        response = r.brpop(["url"])
        extracted_url = response[1]
        print(f"testing {extracted_url}")
        return extracted_url

discovered_robots_file: dict[str, str] = {}
def is_forbidden(url: str):
    parsed_url = urlparse(url)
    robot_url = f"{parsed_url.scheme}://{parsed_url.netloc}/robots.txt"
    robot_file_cached = discovered_robots_file.get(parsed_url.netloc) is not None
    print(f"Getting robots from {robot_url} -> { "cached" if robot_file_cached else "fetching" }")
    
    if not robot_file_cached:
        try:
            robot_file_request_response = requests.get(robot_url, headers={"User-Agent": "Mozilla/5.0"}, timeout=2,)
            robot_file_content = robot_file_request_response.text
            
            if (robot_file_request_response.status_code == 404):
                robot_file_content = ""
            else:
                robot_file_request_response.raise_for_status()
            discovered_robots_file[parsed_url.netloc] = robot_file_content
        except requests.HTTPError as he:
            # Do not bloat the output console with erro messages from all the status code
            return True
        except Exception as e:
            print(f"\tError retrieving robot file from {robot_url} -> ignored")
            print(e)
            return True
        
    robot_file = discovered_robots_file.get(parsed_url.netloc)
    assert robot_file is not None
    rp = RobotFileParser()
    rp.parse(robot_file.splitlines())
    
    return not rp.can_fetch("Mozilla/5.0",url)

def send_batch(conn, batch: list[str], batch_size=10):
    if len(batch) < batch_size:
        return False

    with conn.cursor() as curs:
        # TODO optimizes this command to insert multiple urls at once
        for url in batch:
            curs.execute(
                """
                INSERT INTO pending VALUES (%s) ON CONFLICT DO NOTHING
                """, (url,)
            )
    conn.commit()
    batch.clear()
    print(f"\tbatch sent")
    return True

def get_queue_size(r: redis.Redis):
    return r.llen("url")

def robot(r: redis.Redis, conn):
    authorized_url_batch: list[str] = []
    queue_size = get_queue_size(r)
    while True:
        conn.commit()
        next_url = pop_url(r)
        url_forbiden = is_forbidden(next_url)
        if url_forbiden:
            print("\t*** ROBOT FORBIDDEN ***")
            continue
        
        authorized_url_batch.append(next_url)
        if send_batch(conn, authorized_url_batch):
            queue_size = get_queue_size(r)
        print(f"\tleft: {queue_size}")

def main():
    conn = psycopg2.connect(
    dbname=os.environ.get("DB_NAME", "gogole"),
    user=os.environ.get("DB_USER", "developer"),
    password=os.environ["POSTGRES_PASSWORD"],  # Required, no default
    host=os.environ.get("DB_HOST", "db")
    )
    r = redis.Redis(host="redis", decode_responses=True)
    
    with conn, r:
        r.ping()
        robot(r, conn)

if __name__ == "__main__":
    main()