import requests
import threading
import time
import matplotlib.pyplot as plt

SINGLE_THREAD_PORT = 8080
MULTI_THREAD_PORT = 8081

TEST_THREADS = [
    1, 2, 4, 8, 16, 32, 64, 128
]

def wait_threads_to_finish():
    while threading.active_count() > 1:
        pass

def get_url(url):
    print("GET %s" % url)
    requests.get(url)
    print("GET %s done" % url)
    
def test(port, thread_num):
    url = "http://localhost:%s" % port
    start_time = time.time()
    
    for _ in range(thread_num):
        thread = threading.Thread(target=get_url, args=(url, ))
        thread.start()
        
    wait_threads_to_finish()
    
    return time.time() - start_time

def plot(single_thread_times, multi_thread_times):
    plt.plot(TEST_THREADS, single_thread_times, 'ro-')
    plt.plot(TEST_THREADS, multi_thread_times, 'bo-')
    plt.xlabel('Threads')
    plt.ylabel('Time')
    plt.title('Single thread vs Multi thread')
    plt.legend(['Single thread', 'Multi thread'])
    plt.show()
    
def main():
    single_thread_times = []
    multi_thread_times = []
    
    for thread_num in TEST_THREADS:
        print("Testing with %s threads" % thread_num)
        single_thread_time = test(SINGLE_THREAD_PORT, thread_num)
        multi_thread_time = test(MULTI_THREAD_PORT, thread_num)
        
        single_thread_times.append(single_thread_time)
        multi_thread_times.append(multi_thread_time)
        
        print("Single thread time: %s" % single_thread_time)
        print("Multi thread time: %s" % multi_thread_time)
        
    plot(single_thread_times, multi_thread_times)
        
        
if __name__ == "__main__":
    main()