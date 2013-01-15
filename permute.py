def permute (word):
	result = []
	if len(word) == 1:
        	result.append(word)
	else:
		for pos in range(len(word)):
			permutations = permute(word[0:pos] + word[pos + 1:len(word)])
			for item in permutations:
				result.append(word[pos] + item)
	return result
