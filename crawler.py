# import requests
from urllib.request import urlopen, Request
from urllib.parse import urljoin, urlparse
from bs4 import BeautifulSoup
from timeit import default_timer as timer
from time import sleep
from collections import deque

def PerformCrawling(currentUrl: str):
    print(f"Currently crawling {currentUrl}")
    
    req = Request(currentUrl, headers={"User-Agent": "Mozilla/5.0"})
    
    begin = timer()
    # response = requests.get(currentUrl)
    response = urlopen(req, timeout=30)
    fetchTimeMs = (timer() - begin) * 1000
    links = []
    
    if "text/html" in response.headers.get("Content-Type", ""):
        begin = timer()
        soup = BeautifulSoup(response, "html.parser")
        parseTimeMs = (timer() - begin) * 1000
        print(f"\tfetch: {fetchTimeMs:.2f}ms   parsing: {parseTimeMs:.2f}ms")
        
        for a in soup.find_all("a", href=True):
            href = a["href"].strip()
            if href.startswith(("http", '/')):
                links.append(urljoin(currentUrl, href))
    
    return links

def crawler():
    urls = deque(["http://fr.wikipedia.org/wiki/Hasard"])
    visited = set()
    count = 0
    
    while len(urls) != 0:
        currentUrl = urls.popleft()
        
        if currentUrl in visited:
            continue
        
        visited.add(currentUrl)
        foundUrls = PerformCrawling(currentUrl)
        count += 1
        
        if count < 10:
            exclusiveUrls = [url for url in foundUrls if url not in visited]
            urls.extend(exclusiveUrls)
            print(f"Adding {len(exclusiveUrls)}")
        print(f"remaining: {len(urls)}")
        sleep(1.0)
    
    # p  = soup.select('p')
    # h1 = soup.select('h1')
    # title = soup.select("title")
    
    # print(f"Title:\n{title}")
    # print(f"H1:\n{h1}")
    # print(f"P:\n{p}")

if __name__ == "__main__":
    crawler()