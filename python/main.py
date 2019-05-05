from sympy import Symbol, symbols, false, true
from sympy.logic.boolalg import ITE, And, Xor, Or, Not
from sympy.logic.inference import satisfiable

A = []
N = 0
KB = true
P = 'P'
W = 'W'
direction = [(-1,0), (0,1), (1,0), (0,-1)]
score = 0
assign = {}
exit = (0,0)
MAX_STEP = 150
path = []
step = -1

def init():
	global assign
	global exit
	for i in range(N):
		for j in range(N):
			assign[str(i)+str(j)] = [1,1]
			if ('A' in A[i][j]):
				exit = [i,j]

def inRange(i, j):
	if (i<0 or i>=N or j<0 or j>=N):
		return false
	return true
  
def addToKB(x):
	global KB
	i = x[0]
	j = x[1]
	KB = KB & ~Symbol(P+str(i)+str(j)) & ~Symbol(W+str(i)+str(j))
	if ('S' in A[i][j] and 'B' in A[i][j]):
		orArr = false
		for k in direction:
			if inRange(i+k[0],j+k[1]):
				orArr = orArr | Symbol(W+str(i+k[0])+str(j+k[1]))
		KB = KB & orArr
    
		orArr = false
		for k in direction:
			if inRange(i+k[0],j+k[1]):
				orArr = orArr | Symbol(P+str(i+k[0])+str(j+k[1]))
		KB = KB & orArr
	elif ('S' in A[i][j]):
		orArr = false
		for k in direction:
			if inRange(i+k[0],j+k[1]):
				orArr = orArr | Symbol(W+str(i+k[0])+str(j+k[1]))
		KB = KB & orArr
    
		orArr = false
		for k in direction:
			if inRange(i+k[0],j+k[1]):
				KB = KB & ~Symbol(P+str(i+k[0])+str(j+k[1]))
	elif ('B' in A[i][j]):
		orArr = false
		for k in direction:
			if inRange(i+k[0],j+k[1]):
				orArr = orArr | Symbol(P+str(i+k[0])+str(j+k[1]))
		KB = KB & orArr
    
		orArr = false
		for k in direction:
			if inRange(i+k[0],j+k[1]):
				KB = KB & ~Symbol(W+str(i+k[0])+str(j+k[1]))
	if (A[i][j] == '-' or A[i][j] == 'W' or A[i][j] == 'A' or A[i][j] == 'G'):
		orArr = false
		for k in direction:
			if inRange(i+k[0],j+k[1]):
				KB = KB & ~Symbol(P+str(i+k[0])+str(j+k[1])) & ~Symbol(W+str(i+k[0])+str(j+k[1]))
        
def calculateScore(x):
	global score
	i = x[0]
	j = x[1]
	if ('G' in A[i][j]):
		score = score + 100
    
def bfs(src, des):
	global assign
	res = []
	res.append(des)
	if (src == des):
		return res
	tmp = []
	isVisited = {}
	tmp.append(src)
	isDes = false
	isVisited[str(src[0])+str(src[1])] = src
	while (len(tmp) != 0):
		current = tmp.pop(0)
		for k in direction:
			if (inRange(current[0]+k[0],current[1]+k[1]) 
				and not (str(current[0]+k[0])+str(current[1]+k[1])) in isVisited.keys()
				and assign[str(current[0]+k[0])+str(current[1]+k[1])] == [0,0]):
				isVisited[str(current[0]+k[0])+str(current[1]+k[1])] = [current[0],current[1]]
				tmp.append([current[0]+k[0], current[1]+k[1]])
				if ([current[0]+k[0],current[1]+k[1]] == des):
					isDes = true
		if (isDes):
			break
	while (des != src):
		res.append(isVisited[str(des[0])+str(des[1])])
		des = isVisited[str(des[0])+str(des[1])]
	res.reverse()
	res.pop(0)
	return res

def run():
	global step
	current = exit
	isExplored = []
	isExplored.append(exit)
	start = 0
	while (step < MAX_STEP and start < len(isExplored)):
		newPath = bfs(current,isExplored[start])
		home = bfs(isExplored[start],exit)
		print(str(len(newPath)) + ' ' + str(len(home)) + ' ' + str(step))
		if (2*len(newPath)+len(home)+step<=MAX_STEP):
			for i in newPath:
				path.append(i)
			step = step + len(newPath)
		else:
			for i in home:
				path.append(i)
			step = step + len(home)
			break
		current = isExplored[start]
		addToKB(current)
		tmp = satisfiable(KB)
		for i in tmp:
			x = str(i)[1]
			y = str(i)[2]
			if ('P' in str(i)):
				assign[x+y][0] = 1 if tmp[Symbol(str(i))] else 0
			if ('W' in str(i)):
				assign[x+y][1] = 1 if tmp[Symbol(str(i))] else 0
			if (assign[x+y] == [0,0] 
				and not [int(x,10),int(y,10)] in isExplored):
				isExplored.append([int(x,10),int(y,10)])
		if start < len(isExplored):
			start = start+1
		calculateScore(current)
	
def input():
	global A
	global N
	check = true
	f = open('map.txt', 'r')
	if f.mode == 'r':
		content = f.read().strip().split('\n')
		for i in content:
			if check:
				N = int(i,10)
				check = False
			else:
				A.append(i.split('.'))
		
def main():
	input()
	init()
	run()
	print(score)
	print(step)
	print(path)
	
if __name__ == '__main__':
	main()