package main

import (
    . "fmt"
    "runtime"
    "time"
)

const iterations = 1000000

var accumulator int

func increment() {
    for i := 0; i < iterations; i++ {
        accumulator++
    }
}

func decrement() {
    for i := 0; i < iterations; i++ {
        accumulator -= 1
    }
}

func main() {
    runtime.GOMAXPROCS(runtime.NumCPU())

    go increment()
    go decrement()

    // There is no way to wait for the completion of a
    // goroutine. This would require explicit communication.
    // Like in the hello-world example for this excercise,
    // we do a rather shady wait using sleep.
    time.Sleep(100 * time.Millisecond)

    Println(accumulator)
}
