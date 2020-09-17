#!/usr/bin/env python

db = {}

def newuser():
	prompt = 'register: '
	while True:
		name = raw_input(prompt)
		if db.has_key(name):
			prompt = 'name taken, try another: '
			continue
		else:
			break
	pwd = raw_input('passwd: ')
	db[name] = pwd

def olduser():
	name = raw_input('login: ')
	pwd = raw_input('passwd: ')
	passwd = db.get(name)
	if passwd == pwd:
		print 'login success:', name
	else:
		print 'login failure'

def showmenu():
	prompt = """
(R)eister
(L)ogin
(Q)uit
Enter choice: """

	done = False
	while not done:
		chosen = False
		while not chosen:
			try:
				choice = raw_input(prompt).strip()[0].lower()
			except (EOFError, KeyboardInterrupt):
				choice = 'q'
			print '\nYour choice: [%s]' %choice
			if choice not in 'rlq':
				print 'invalid option, try again'
			else:
				chosen = True
				if choice == 'r':
					newuser()
				elif choice == 'l':
					olduser()
				else:
					done = True

if __name__ == '__main__':
	showmenu()
