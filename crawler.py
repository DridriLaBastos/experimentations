# import requests
from urllib.request import urlopen, Request
from urllib.parse import urljoin, urlparse
from bs4 import BeautifulSoup
from timeit import default_timer as timer
from time import sleep
from collections import deque
from random import randint

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
    urls = ["http://fr.wikipedia.org/wiki/Hasard"]
    visited = set()
    count = 0
    _pass = 0
    
    while len(urls) != 0:
        currentUrl = urls.pop(randint(0, len(urls) - 1))
        count += 1
        _pass += 1
        
        if currentUrl in visited:
            continue
        
        foundUrls = PerformCrawling(currentUrl)
        visited.add(currentUrl)
        
        disp = f"[{_pass}] "
        if len(urls) < 10000:
            exclusiveUrls = [url for url in foundUrls if url not in visited]
            inclusiveUrs  = [url for url in foundUrls if url     in visited]
            urls.extend(exclusiveUrls)
            disp += f"{len(foundUrls)}/{len(exclusiveUrls)}/{len(inclusiveUrs)} "
        disp += f"remaining: {len(urls)}"
        print(disp)
        sleep(0.1)

if __name__ == "__main__":
    crawler()