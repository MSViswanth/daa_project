def auctionAlgorithm(setA, setB):
    m = len(setA)
    n = len(setB)
    # assignment = [[0] * n for _ in range(m)]
    assignment = [[0 for i in range(n)] for j in range(m)]
    prices = [0 for i in range(n)]
    while True:
        bids = calculateBids(setA, setB, prices)
        maxBidIndices = findMaxBidIndices(bids)
        newAssignment = [[0 for i in range(n)] for j in range(m)]
        for i in range(m):
            newAssignment[i][maxBidIndices[i]] = 1
        if assignment == newAssignment:
            break
        assignment = newAssignment
        prices = updatePrices(prices, maxBidIndices, bids)
    totalDistance = calculateTotalDistance(setA, assignment, setB)
    return assignment, totalDistance

def calculateBids(setA, setB, prices):
    m = len(setA)
    n = len(setB)
    bids = [[0] * n for _ in range(m)]
    for i in range(m):
        for j in range(n):
            bids[i][j] = calculateBid(setA[i], setB[j], prices[j])
    return bids

def calculateBid(a, b, price):
    distance = 0
    for i in range(len(a)):
        diff = a[i] - b[i]
        distance += diff * diff
    return -distance - price

def findMaxBidIndices(bids):
    m = len(bids)
    n = len(bids[0])
    maxBidIndices = [0] * m
    for i in range(m):
        maxIndex = 0
        maxBid = bids[i][0]
        for j in range(1, n):
            if bids[i][j] > maxBid:
                maxBid = bids[i][j]
                maxIndex = j
        maxBidIndices[i] = maxIndex
    return maxBidIndices

def updatePrices(prices, maxBidIndices, bids):
    n = len(prices)
    for j in range(n):
        maxBidForJ = float('-inf')
        for i in range(len(maxBidIndices)):
            if maxBidIndices[i] == j:
                maxBidForJ = max(maxBidForJ, bids[i][j])
        prices[j] += maxBidForJ
    return prices

def calculateTotalDistance(setA, assignment, setB):
    totalDistance = 0
    for i in range(len(setA)):
        assignedColumn = -1
        for j in range(len(setB)):
            if assignment[i][j] == 1:
                assignedColumn = j
                break
        if assignedColumn != -1:
            totalDistance += calculateDistance(setA[i], setB[assignedColumn])
    return totalDistance

def calculateDistance(a, b):
    distance = 0
    for i in range(len(a)):
        diff = a[i] - b[i]
        distance += diff * diff
    return distance ** 0.5

def printSet(set):
    for row in set:
        print(row)

setA = [
    [3, 2],
    [4, 1],
    [8, 5]
]
setB = [
    [1, 2],
    [3, 6],
    [1, 5]
]
result = auctionAlgorithm(setA, setB)
assignment = result[0]
totalDistance = result[1]
print("Set A:")
printSet(setA)
print("Set B:")
printSet(setB)
print("Assignment Matrix:")
printSet(assignment)
print("Total Distance: " + str(totalDistance))