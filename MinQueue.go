package main

import "fmt"

const (
	R uint32 = 4294967295 // 2^32
	A uint32 = 1664525
	C uint32 = 1013904223
)

func randomGenerator(currRand uint32) uint32 {
	return (A*currRand + C) % R
}

// Dynamic Array + Operations

type DynamicArray struct {
	data     []uint32
	size     uint32
	capacity uint32
}

func (da *DynamicArray) insertDA(elemToInsert uint32) {

	if da.size == da.capacity {
		da.capacity *= 2
		da.data = append(da.data, da.data...)
	}

	da.data[da.size] = elemToInsert
	da.size++
}

func (da *DynamicArray) removeDA(idxRemove uint32) {

	if da.size == (da.capacity / 4) {
		da.capacity /= 2
		da.data = da.data[:da.capacity]
	}

	if idxRemove == 0 {
		if da.size > 1 {
			da.data = da.data[1:]
			da.data = append(da.data, 0)
		}
	}

	da.size--
}

// MinQueue + Operations

type MinQueue struct {
	V        DynamicArray
	M        DynamicArray
	deqCount uint32
}

func (mq *MinQueue) minQueueEnqueue(elemToEnqueue uint32) {
	mq.V.insertDA(elemToEnqueue)

	for mq.M.size > 0 && mq.V.data[mq.M.data[mq.M.size-1]-mq.deqCount] > elemToEnqueue {
		mq.M.removeDA(mq.M.size - 1)
	}

	mq.M.insertDA(mq.V.size - 1 + mq.deqCount)
}

func (mq *MinQueue) minQueueDequeue() {
	if mq.V.data[0] == mq.V.data[mq.M.data[0]-mq.deqCount] {
		mq.M.removeDA(0)
	}
	mq.V.removeDA(0)
}

func main() {

	var S, B, N, P uint32
	fmt.Scan(&S, &B, &N, &P)

	var currentRand uint32 = S

	var Q MinQueue
	Q.V.data = make([]uint32, 1)
	Q.V.capacity = 1
	Q.M.data = make([]uint32, 1)
	Q.M.capacity = 1

	var burnIn uint32
	for burnIn = 0; burnIn < B; burnIn++ {
		if burnIn == 0 {
			Q.minQueueEnqueue(currentRand)
			continue
		}
		currentRand = randomGenerator(currentRand)
		Q.minQueueEnqueue(currentRand)
	}

	var operation uint32
	for operation = 0; operation < N; operation++ {
		currentRand = randomGenerator(currentRand)
		if currentRand%10 < P {
			currentRand = randomGenerator(currentRand)
			Q.minQueueEnqueue(currentRand)
		} else {
			Q.minQueueDequeue()
			Q.deqCount++
		}

		fmt.Println(Q.M.size, Q.M.data[0]-Q.deqCount)

	}

}
