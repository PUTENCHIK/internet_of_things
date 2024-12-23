a = {
    0: 4,
    1: 47,
    2: 72,
    3: 10,
    4: 35,
    5: 18,
    6: 16,
    7: 15,
    8: 0,
    9: 2
}

strs = [bin(value)[2:] for key, value in a.items()]
strs = [f"B1{'0'*(7 - len(s))}{s}" for s in strs]

for i in strs:
    print(i)
