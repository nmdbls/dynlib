from mylib import benchmark_subtract

import time

def list_subtract(a, b):
    return [a[i] - b[i] for i in range(len(b))]
    
    
def python_benchmark(a, b, iterations):
    start = time.time()
    
    for i in range(iterations):
        list_subtract(a, b)
        
    return time.time() - start
    
    
benchmarks = [10000, 100000, 1000000]

a = list(range(100))
b = list(range(100))

for i in benchmarks:
    print(f"python {i} iterations: {python_benchmark(a, b, i):.2f} s")
    print(f"cpp {i} iterations: { benchmark_subtract(a, b, i):.2f} s")
    
    