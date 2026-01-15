import requests
import psycopg2
import os
from urllib.parse import urljoin
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
    response = requests.get(url, headers={"User-Agent": "Mozilla/5.0"}, timeout=30)
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
            url = fetch_next_url(conn)
            links = explore_url(url, conn)
            unvisited_size = get_unvisited_size(conn)
            total_size = get_total_size(conn)
            
            if unvisited_size < 1000:
                insert_links(conn, links)
                
            print(f"\t{unvisited_size}/{total_size}")
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