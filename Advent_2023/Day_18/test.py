import math

#S='data'
f=open("input.txt",'r')
R=f.read()
L=R.split("\n")
Block=R.split("\n\n")
f.close()

steps=[]
#direcmap={"R":(0,1), "D":(1,0), "L":(0,-1), "U":(-1,0)}
direcmap={"0":(0,1), "1":(1,0), "2":(0,-1), "3":(-1,0)}

for line in L[:-1]:
    direc, length, col = line.split(" ")
    direc = line.split(" ")[-1][-2]     #comment out for part 1
    # length = line.split(" ")[-1][-7:-2] #comment out for part 1
    length = int(length, 16)            #just do int(length) for part 1
    steps.append((direc,length))

x=0
y=0
perimeter=0
area=0
for step in steps:
    direc, length = step
    dy, dx = direcmap[direc]
    dy, dx = dy*length, dx*length
    y, x = y+dy, x+dx
    perimeter+=length
    area+=x*dy
print('Inner area:',area)
print('Perimeter: ',perimeter)
print("Total area:",area+perimeter//2+1)

