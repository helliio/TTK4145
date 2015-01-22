from threading import Thread

i = 0
iterations = 1000000

def increase():
	global i
	for x in range(iterations):
		i = i + 1

def decrease():
	global i
	for x in range(iterations):
		i = i - 1

def main():
	thread_inc = Thread(target = increase, args = (),)
	thread_dec = Thread(target = decrease, args = (),)
	thread_inc.start()
	thread_dec.start()
	thread_inc.join()
	thread_dec.join()
	print(i)
	
if __name__ == "__main__":
	main()
