import requests
import psycopg2
import os
from urllib.parse import urljoin, urlparse
from urllib.robotparser import RobotFileParser
from bs4 import BeautifulSoup
from timeit import default_timer as timer
from time import sleep
import redis

DISCOVERING_BATCH_SIZE = 50
def fetch_url_batch_set(conn, batch_size = DISCOVERING_BATCH_SIZE):
    # TODO: By doing so, if an error arrives during the execution of the program the remaning urls are lost
    with conn.cursor() as curs:
        curs.execute("""
                     DELETE FROM pending WHERE url IN (SELECT url FROM pending LIMIT %s FOR UPDATE SKIP LOCKED) RETURNING url
                     """, (batch_size,))
        return [fetch_result[0] for fetch_result in curs.fetchall()]

def fetch_next_url(conn):
    with conn.cursor() as curs:
        curs.execute("""
                     DELETE FROM pending WHERE url IN (SELECT url FROM pending LIMIT 1 FOR UPDATE SKIP LOCKED) RETURNING url
                     """)
        next_url = curs.fetchone()[0]
        print(f"Crawling {next_url}")
        return next_url

discovered_robots_file: dict[str, str] = {}
def is_forbiden(url: str):
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
            if he.response.status_code is None:
                # Propagate to next to display the error if exception not related to http status
                raise he
            else:
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

def explore_url(url: str, conn):
    begin = timer()
    response = requests.get(url, headers={"User-Agent": "Mozilla/5.0"}, timeout=10)
    fetchTimeMs = (timer() - begin) * 1000
    links = []
    
    content = ""
    
    if "text/html" in response.headers.get("Content-Type", ""):
        begin = timer()
        soup = BeautifulSoup(response.text, "html.parser")
        parseTimeMs = (timer() - begin) * 1000
        print(f"\tfetch: {fetchTimeMs:.2f}ms   parsing: {parseTimeMs:.2f}ms")
        
        for a in soup.find_all("a", href=True):
            href = a["href"].strip()
            if href.startswith(("http", '/')):
                links.append(urljoin(url, href))
    
        content = " ".join(soup.get_text(separator=' ').lower().split())
        
    # TODO: The whole thing is not thread safe
    with conn.cursor() as curs:
        # curs.execute("UPDATE crawling SET content = %s WHERE url = %s", (content, url,))
        curs.execute("INSERT INTO crawling VALUES (%s,%s) ON CONFLICT DO NOTHING", (url, content, ))
    return links

def push_links(r: redis.Redis, links: list[str]):
    # * to unpacke the list and send it as multiple arguments
    if len(links) > 0:
        r.lpush("url",*links)

def get_unvisited_size(conn):
    with conn.cursor() as curs:
        curs.execute("SELECT Count(url) FROM pending")
        return curs.fetchone()[0]

def get_total_size(conn):
    with conn.cursor() as curs:
        curs.execute("SELECT Count(url) FROM crawling")
        return curs.fetchone()[0]

def insert_links(conn, links: list[str]):
    # TODO: Optimizes this
    with conn.cursor() as curs:
        for link in links:
            curs.execute(""" 
                        INSERT INTO pending VALUES (%s) ON CONFLICT (url) DO NOTHING
                        """, (link,))

def crawler(conn, redis):
    while True:
        with conn:
            begin = timer()
            crawling_url = fetch_next_url(conn)
            
            # The script 'robots.py' check for the ability of the url in robots.txt of the file
            # Every url put into the pending database is guaranteed to be available to fetch
            # Thus it is not necessary anymore to check the robots file for the url fetcched
            # WARNING:  Is it a possible case that an available URL at the time robots.py
            #           checked it becomes unavailable when we fetches it from here ?
            #           seems mostly unpossible 
            linked_url = explore_url(crawling_url, conn)
            push_links(redis, linked_url)
            
            unvisited_size = get_unvisited_size(conn)
            total_size = get_total_size(conn)
            
            if unvisited_size < 5000:
                insert_links(conn, linked_url)
            elapsed = timer() - begin
            print(f"\t{unvisited_size}/{total_size}/{len(linked_url)}")
            print(f"\t{elapsed:.3f}s")
            sleep(0.100)

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
        crawler(conn, r)

if __name__ == "__main__":
    main()