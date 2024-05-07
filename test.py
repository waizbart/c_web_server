import requests
import threading
import time

THREADS = 1000

def wait_threads_to_finish():
    while threading.active_count() > 1:
        pass

def get_url(url):
    print("GET %s" % url)
    requests.get(url)
    print("GET %s done" % url)
    
def main():
    url = "http://localhost:8080"
    start_time = time.time()
    
    for _ in range(THREADS):
        thread = threading.Thread(target=get_url, args=(url, ))
        thread.start()
        
    wait_threads_to_finish()
    
    print("Execution time: %s" % (time.time() - start_time))
        
        
if __name__ == "__main__":
    main()