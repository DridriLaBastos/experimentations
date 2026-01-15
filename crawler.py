import requests
import psycopg2
import os
from urllib.parse import urljoin, urlparse
from urllib.robotparser import RobotFileParser
from bs4 import BeautifulSoup
from timeit import default_timer as timer
from time import sleep

def fetch_next_url(conn):
    with conn.cursor() as curs:
        curs.execute("""
                 WITH not_visited AS (SELECT url FROM crawling WHERE content IS NULL)
                 SELECT url FROM not_visited OFFSET Random(0,(
	                SELECT Count(url) FROM not_visited) - 1)
                 LIMIT 1;
                 """)
        return curs.fetchone()[0]

def explore_url(url: str, conn):
    print(f"Currently crawling {url}")
    
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
        curs.execute("UPDATE crawling SET content = %s WHERE url = %s", (content, url,))
    return links

discovered_robots_file: dict[str, str] = {}
def remove_robots(links: list[str]):
    updated_links = []
    
    for url in links:
        parsed_url = urlparse(url)
        
        if discovered_robots_file.get(parsed_url.netloc) is None:
            robot_url = f"{parsed_url.scheme}://{parsed_url.netloc}/robots.txt"
            try:
                robot_file_request_response = requests.get(robot_url, timeout=2)
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
                    continue
            except Exception as e:
                print(f"\tError retrieving robot file from {url} -> ignored")
                print(e)
                continue
        
        robot_file = discovered_robots_file.get(parsed_url.netloc)
        assert robot_file is not None
        rp = RobotFileParser()
        rp.parse(robot_file.splitlines())
        
        canfetch = rp.can_fetch("Mozilla/5.0",url)
        if canfetch:
            updated_links.append(url)
            
    return (updated_links, len(links) - len(updated_links))

def get_unvisited_size(conn):
    with conn.cursor() as curs:
        curs.execute("SELECT Count(url) FROM crawling WHERE content IS NULL")
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
                        INSERT INTO crawling VALUES (%s,null) ON CONFLICT (url) DO NOTHING
                        """, (link,))

def crawler(conn):
    while True:
        with conn:
            begin = timer()
            url = fetch_next_url(conn)
            total_links = explore_url(url, conn)
            links, removed_count = remove_robots(total_links)
            unvisited_size = get_unvisited_size(conn)
            total_size = get_total_size(conn)
            
            if unvisited_size < 5000:
                insert_links(conn, links)
            elapsed = timer() - begin
            print(f"\tGot {len(total_links)} adding {len(links)} removed {removed_count}")
            print(f"\t{unvisited_size}/{total_size}")
            print(f"\t{elapsed:.3f}s")
            sleep(0.100)

def main():
    conn = psycopg2.connect(
        dbname=os.environ.get("DB_NAME", "gogole"),
        user=os.environ.get("DB_USER", "developer"),
        password=os.environ["POSTGRES_PASSWORD"],  # Required, no default
        host=os.environ.get("DB_HOST", "db")
    )
    try:
        crawler(conn)
    finally:
        conn.close()

if __name__ == "__main__":
    main()