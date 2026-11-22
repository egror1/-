"""Compare reported roots to an independent bisection reference."""
import math
import re
import subprocess
import sys

executable=sys.argv[1]
checks=0

def run(data):
    result=subprocess.run([executable],input=data,text=True,capture_output=True,timeout=5)
    assert result.returncode==0,result.stderr
    return result.stdout

def check(condition,context):
    global checks
    assert condition,context
    checks+=1

def reference(a,left,right):
    f=lambda x:x-a*math.cos(x)
    for _ in range(100):
        middle=(left+right)/2
        if (f(left)<0)==(f(middle)<0):
            left=middle
        else:
            right=middle
    return (left+right)/2

for a in [-1,-0.5,0,0.25,0.8,1]:
    expected=reference(a,-2,2)
    for tolerance in [1e-6,1e-8,1e-12]:
        for method in [1,2,3]:
            args='-2\n2' if method==2 else '0.5'
            output=run(f'{method}\n{a}\n{tolerance}\n{args}\nn\n')
            match=re.search(r'Корень x = ([\d.eE+-]+)',output)
            check(match is not None,(a,tolerance,method,output))
            actual=float(match[1])
            check(abs(actual-expected)<=tolerance+1e-12,(actual,expected,method,tolerance))
            check(abs(actual-a*math.cos(actual))<=2*tolerance+2e-12,(actual,a,tolerance))
            iterations=int(re.search(r'N = (\d+)',output)[1])
            check(0<=iterations<100000,output)
for tolerance in [1e-6,1e-8]:
    output=run(f'1\n10\n{tolerance}\n0.5\nn\n')
    check('Нет сходимости за 99999' in output and 'Корень x =' not in output,output)
    for method in [2,3]:
        args='0\n2' if method==2 else '0.5'
        output=run(f'{method}\n10\n{tolerance}\n{args}\nn\n')
        actual=float(re.search(r'Корень x = ([\d.eE+-]+)',output)[1])
        check(abs(actual-reference(10,0,2))<tolerance+1e-12,output)
for data in ['2\n1\n1e-8\n2\n3\nn\n','2\n1\n1e-8\n1\n0\nn\n','2\n1\n1e-8\n1\n1\nn\n','1\n1\n0\nn\n','1\n1\n-1e-6\nn\n','3\nnan\nn\n','3\n1e309\nn\n','3\n1 tail\nn\n','3.5\nn\n','9\nn\n']:
    check('Ошибка' in run(data),data)
output=run(f'3\n1\n1e-8\n{-math.pi/2}\nn\n')
check('производная близка к нулю' in output,output)
output=run('1\n0\n1e-8\n0\nn\n')
check('N = 0' in output and 'Корень x = 0.' in output,output)
output=run('2\n0\n1e-8\n0\n2\nn\n')
check('N = 0' in output and 'Корень x = 0.' in output,output)
output=run('1\n1\n1e-6\n0.5\ny\n2\n1\n1e-8\n0\n1\ny\n3\n1\n1e-8\n0.5\nn\n')
check(output.count('Корень x =')==3,output)
output=run('1\ntext\ny\n3\n1\n1e-8\n0.5\nn\n')
check('Ошибка' in output and 'Корень x =' in output,output)
for data in ['', '1\n', '2\n1\n1e-8\n', '3\n1\n1e-8\n0.5\nwrong\ny\n0\n']:
    check(bool(run(data)),data)
print(f'PASS: {checks} checks (three methods, precision, divergence, intervals, derivative and menu)')
