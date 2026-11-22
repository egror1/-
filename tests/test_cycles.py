"""Black-box checks against direct sums, factorials and nested radicals."""
import math
import random
import re
import subprocess
import sys

executable = sys.argv[1]
checks = 0

def run(data):
    result = subprocess.run([executable], input=data, text=True, capture_output=True, timeout=5)
    assert result.returncode == 0, result.stderr
    return result.stdout

def check(condition, context):
    global checks
    assert condition, context
    checks += 1

rng = random.Random(11)
for n, m in [(2,1),(5,1),(10,5),(20,3),(1000000,7)] + [(rng.randint(6,10000),rng.randint(1,5)) for _ in range(30)]:
    output=run(f'1\n{n}\n{m}\nn\n')
    expected=sum(i for i in range(1,n+1) if i%5==0 and i%m!=0)
    check(int(re.search(r'Сумма = (\d+)',output)[1])==expected,(n,m,output))
for a in [-1e100,-10,-0.1,0,1,4.5,147456,1e10]:
    output=run(f'2\n{a}\nn\n')
    actual=float(re.search(r'Произведение S = ([\d.eE+-]+)',output)[1])
    expected=math.prod(i*i for i in range(2,9,2))-a if a>=0 else math.prod(i-2 for i in range(3,10,3))
    check(math.isclose(actual,expected,rel_tol=1e-12,abs_tol=1e-8),(a,actual,expected))
output=run('3\nn\n')
rows=re.findall(r'^\s*(0\.\d+|1\.\d+)\s+(-?[\d.]+)\s+(-?[\d.]+)\s+(\d+)\s*$',output,re.M)
check(len(rows)==6,output)
for index,(x,y,s,n) in enumerate(rows):
    x,y,s,n=float(x),float(y),float(s),int(n)
    expected=(1-x*x/2)*math.cos(x)-x/2*math.sin(x)
    direct=sum((-1)**k*(2*k*k+1)*x**(2*k)/math.factorial(2*k) for k in range(n+1))
    check(abs(x-index*0.2)<1e-9,(x,index))
    check(abs(y-expected)<1e-9,(y,expected))
    check(abs(s-expected)<1e-6,(s,expected))
    check(abs(s-direct)<1e-9,(s,direct))
    term=abs((2*n*n+1)*x**(2*n)/math.factorial(2*n))
    check(term<1e-6,(n,term))
for n in [1,2,3,5,10,11,100,10000]:
    output=run(f'4\n{n}\nn\n')
    expected=0
    for k in range(1,n+1):
        expected=math.sqrt(2*k+expected)
        if k in (3,5,10):
            actual=float(re.search(fr'Итерация {k}: y = ([\d.]+)',output)[1])
            check(abs(actual-expected)<1e-9,(n,k,actual,expected))
    actual=float(re.search(r'Результат y = ([\d.]+)',output)[1])
    check(abs(actual-expected)<1e-9,(n,actual,expected))
for data in ['1\n0\nn\n','1\n10\n0\nn\n','1\n10\n10\nn\n','4\n-1\nn\n','4\n1.5\nn\n','2\nnan\nn\n','2\n1e309\nn\n','2\n2 text\nn\n','abc\nn\n','9\nn\n']:
    check('Ошибка' in run(data),data)
output=run('4\n3\ny\n4\n5\ny\n4\n10\nn\n')
check(output.count('Результат y =')==3,output)
output=run('4\ntext\ny\n3\nn\n')
check('Ошибка' in output and 'Y(x)' in output,output)
for data in ['', '1\n','4\n','invalid','3\n','4\n1\nwrong\ny\n0\n']:
    check(bool(run(data)),data)
print(f'PASS: {checks} checks (variant 11, all four tasks, input recovery and repeat execution)')
