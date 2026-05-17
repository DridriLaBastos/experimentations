import os
import psycopg2
import requests

from psycopg2.extras import execute_values

from urllib.parse import urljoin,urlparse
from urllib.robotparser import RobotFileParser

from bs4 import BeautifulSoup
from time import sleep
from timeit import default_timer as timer

def fetch_next_url(conn):
    with conn.cursor() as curs:
        curs.execute("""
                     SELECT url FROM pending LIMIT 1 FOR UPDATE SKIP LOCKED
                     """)
        next_url = curs.fetchone()
        return next_url[0]

discovered_robot_files: dict[str,str] = {}
def can_fetch(url: str):
    parsed_url = urlparse(url)
    robot_url = f"{parsed_url.scheme}://{parsed_url.netloc}/robots.txt"
    robot_file_cached = discovered_robot_files.get(parsed_url.netloc) is not None
    print(f"Getting robots from {robot_url} -> {"cached" if robot_file_cached else "fetching"}")
    
    if not robot_file_cached:
        try:
            robot_file_request_response = requests.get(robot_url, headers={"User-Agent": "Mozilla/5.0"}, timeout=2)
            robot_file_content = robot_file_request_response.text
            
            # Missing or empty robot file indicates that agent can access everything on the site (trsuted IA on this)
            if robot_file_request_response.status_code == 404:
                robot_file_request_response.status_code = 200
                robot_file_content = ""
            
            robot_file_request_response.raise_for_status()
            
            discovered_robot_files[parsed_url.netloc] = robot_file_content
        except requests.HTTPError:
            # Do not bloat the output console with error messages from all the status code
            return False
        except Exception as e:
            print(f"\tError retrieving robot file from {robot_url} -> ignored")
            return False
    
    robot_file_content = discovered_robot_files.get(parsed_url.netloc)
    
    # Sanity check, should not be None at this stage
    assert robot_file_content is not None
    rp = RobotFileParser()
    rp.parse(robot_file_content.splitlines())
    return rp.can_fetch("Mozilla/5.0",url)
    

def fetch_url_content(url):
    begin = timer()
    response = requests.get(url, headers={"User-Agent": "Mozilla/5.0"}, timeout=10)
    fetchTimeS = timer() - begin
    response.raise_for_status()
    return (response.text, fetchTimeS)

def parse_url(url):
    text = ""
    neighbors = []
    urlContent, fetchTimeS = fetch_url_content(url)
    soup = BeautifulSoup(urlContent,"html.parser")
    text = soup.get_text(strip=True, separator=' ')
    
    for a in soup.find_all("a", href=True):
        href = a["href"].strip()
        if href.startswith(("http",'/')):
            neighbors.append(urljoin(url,href))
    return text, neighbors, fetchTimeS

def upload_neighbors(links: list[str], conn):
    # TODO: Optimize this -> check 'execute_values' at https://www.psycopg.org/docs/extras.html#fast-execution-helpers
    with conn.cursor() as cur:
        execute_values(cur, "INSERT INTO pending (url) VALUES %s ON CONFLICT (url) DO NOTHING", [(link,) for link in links])

def upload_content(url: str, content: str, conn):
    with conn.cursor() as curs:
        curs.execute("INSERT INTO crawled (url,content) VALUES (%s,%s)",((url,content)))
    
def crawler_step(conn):
    crawlingUrl = fetch_next_url(conn)
    print(f"Crawling {crawlingUrl}")
    authorized = can_fetch(crawlingUrl)
    
    if not authorized:
        print(f"Unauthorized -> skipping")
        return
    
    content, neighbors, fetchTimeS = parse_url(crawlingUrl)
    print(f"Fetch Time : {fetchTimeS:.3}s")
    upload_neighbors(neighbors, conn)
    upload_content(crawlingUrl, content, conn)

def crawler(conn):
    while True:
        with conn:
            crawler_step(conn)
        sleep(0.100)

def main():
    conn = psycopg2.connect(os.environ.get('POSTGRES_PENDING_URL'))
    crawler(conn)
    

if __name__ == "__main__":
    main()