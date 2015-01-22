from threading import Thread

i = 0
iterations = 1000000


class Concurrent(Thread):
	
	def __init__(self, type, iterations):
		Thread.__init__(self)
		self.type = type
		self.iterations = iterations
	
	def increase(self):
		global i
		for x in range(self.iterations):
			i = i + 1

	def decrease(self):
		global i
		for x in range(self.iterations):
			i = i - 1
	
	def run(self):
		if self.type == "inc":
			self.increase()
		if self.type == "dec":
			self.decrease()
def main():
	thread_inc = Concurrent("inc", iterations)
	thread_dec = Concurrent("dec", iterations)
	thread_inc.start()
	thread_dec.start()
	thread_inc.join()
	thread_dec.join()
	print(i)
	
if __name__ == "__main__":
	main()
