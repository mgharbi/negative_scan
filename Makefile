setup:
	$(MAKE) -C vendors

equations.pdf:
	pandoc equations.md -s -o equations.pdf
