from sympy import Symbol, symbols, false, true
from sympy.logic.boolalg import ITE, And, Xor, Or, Not
from sympy.logic.inference import satisfiable
import turtle

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
		if start == len(isExplored):
			home = bfs(current,exit)
			for i in home:
				path.append(i)
			step = step + len(home)
	
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
	f.close()

class Pen(turtle.Turtle):
	def __init__(self):
		turtle.Turtle.__init__(self)
		self.shape("square")
		self.color("white")
		self.penup()
		self.speed(0)

class Wumpus(turtle.Turtle):
	def __init__(self, x, y):
		turtle.Turtle.__init__(self)
		self.shape("square")
		self.color("red")
		self.penup()
		self.speed(0)
		self.goto(x, y)

class Pit(turtle.Turtle):
	def __init__(self, x, y):
		turtle.Turtle.__init__(self)
		self.shape("square")
		self.color("black")
		self.penup()
		self.speed(0)
		self.goto(x, y)

class Gold(turtle.Turtle):
	def __init__(self, x, y):
		turtle.Turtle.__init__(self)
		self.shape("square")
		self.color("yellow")
		self.penup()
		self.speed(0)
		self.goto(x, y)
	
	def destroy(self):
		self.goto(2000, 2000)
		self.hideturtle()

class Agent(turtle.Turtle):
	def __init__(self):
		turtle.Turtle.__init__(self)
		self.shape("turtle")
		self.color("blue")
		self.penup()
		self.speed(1)

	def is_collision(self, other):
		if self.xcor() == other.xcor() and self.ycor() == other.ycor():
			return True
		return False

def setup():
	world = turtle.Screen()
	world.bgcolor('pink')
	world.title('Wumpus world')
	world.setup(500,500)

	wumpus = []
	pit = []
	gold = []

	pen = Pen()
	agent = Agent()

	for i in range(N):
		for j in range(N):
			if 'P' in A[i][j]:
				pit.append(Pit(i*24,j*24))
			elif 'W' in A[i][j]:
				wumpus.append(Wumpus(i*24,j*24))
			elif 'G' in A[i][j]:
				gold.append(Gold(i*24,j*24))
			else:
				pen.goto(i*24,j*24)
				pen.stamp()

	for i in path:
		agent.goto(i[0]*24,i[1]*24)
		for g in gold:
			if agent.is_collision(g):
				g.destroy()
				gold.remove(g)
				pen.goto(agent.xcor(),agent.ycor())
				pen.stamp()

def output():
	outputPath = []
	for i in path:
			outputPath.append([i[0]+1,i[1]+1])
	with open('result.txt','w') as f:
		f.write('Score = '+str(score)+'\n')
		f.write('Path = ')
		f.write(str(outputPath))

def description():
	print('Blue: Agent')
	print('Red: Wumpus')
	print('Yellow: Gold')
	print('Black: Pit')

def main():
	input()
	init()
	run()
	description()
	setup()
	output()

if __name__ == '__main__':
	main()
	