from threading import Thread, Lock

i = 0
iterations = 1000000


class Concurrent(Thread):
	
	"""
	Thread doing the iterarions.
	"""
	
	def __init__(self, type, iterations,lock):
		"""
		Constructor
		"""
		Thread.__init__(self)
		self.type = type
		self.lock = lock
		self.iterations = iterations
	
	def increase(self):
		"""
		Increment global i
		"""
		global i
		for x in range(self.iterations):
			self.lock.acquire()
			i = i + 1
			self.lock.release()

	def decrease(self):
		"""
		decrement global i
		"""
		global i
		for x in range(self.iterations):
			self.lock.acquire()
			i = i - 1
			self.lock.release()

	
	def run(self):
		"""
		Thread run function
		"""
		if self.type == "inc":
			self.increase()
		if self.type == "dec":
			self.decrease()
def main():
	"""
	Main function
	"""
	lock = Lock()
	thread_inc = Concurrent("inc", iterations, lock)
	thread_dec = Concurrent("dec", iterations, lock)
	thread_inc.start()
	thread_dec.start()
	thread_inc.join()
	thread_dec.join()
	print(i)
	
if __name__ == "__main__":
	main()
