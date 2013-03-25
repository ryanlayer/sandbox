def powerset(lst):
	return reduce(lambda result, \
		x: result + [subset + [x] for subset in result], lst, [[]])

p = powerset(range(3))

t = sorted(p, key = lambda x: (len(x),str(x)))

print t
