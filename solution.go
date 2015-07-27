package main

import (
	"bufio"
	"fmt"
	"io"
	"strings"
)

// Color is the type used to represent
// the color of a vertex.
type Color int

// Node is a node part of a Graph.
type Node struct {
	// Partition refers to the index of the
	// partition in Graph.Partition that this
	// node belongs to. It is redundant
	// since Graph.Partitions could be checked
	// to find out the partition index of any
	// Node, but sticks around here for quick
	// lookup.
	Partition int
}

type Graph struct {
	Nodes []Node

	// Partitions groups the nodes of
	// this Graph by their partition index.
	// The slices here point to the same
	// memory as Nodes does, but are here
	// in order to quickly iterate over
	// partitions.
	Partitions [][]Node

	// Edges holds the adjacent nodes
	// of any given node.
	// This slice is indexed via the index of
	// a specific node in Nodes and values are
	// to be interpreted as indexes for Nodes
	// as well.
	// Beware that because we handle undirected
	// edges here, you will find an edge (a, b)
	// twice (a, b) and (b, a)
	Edges [][]int
}

type Solution struct {
	Graph         *Graph
	Coloring      []Color
	SelectedNodes []int
}

func (s *Solution) Read(rs io.ReadSeeker) (err error) {
	br := bufio.NewReader(rs)

	b, err := br.Peek(1)
	firstByte := b[0]

	firstLine, err := br.ReadString('\n')

	rs.Seek(0, 0)

	// First byte is a digit (ASCII)
	if firstByte > 47 && firstByte < 58 {
		if strings.Contains(firstLine, " ") {
			err = s.ReadPcp(rs)
		} else {
			panic("cannot parse input assumed to be of format .col.b")
		}
	} else {
		panic("cannot parse input assumed to be of format .col")
	}
	return
}

func (s *Solution) ReadPcp(r io.Reader) (err error) {
	var nv, ne, np int
	fmt.Fscanln(r, &nv, &ne, &np)
	fmt.Println(nv, ne, np)

	nodes := make([]Node, nv)
	partitions := make([][]Node, np)
	edges := make([][]int, nv)

	avgEdges := ne / nv

	var prev, cur, p, last int
	fmt.Fscanln(r, &prev)
	edges[0] = make([]int, 0, avgEdges)

	for i := 1; i < nv; i++ {
		edges[i] = make([]int, 0, avgEdges)
		fmt.Fscanln(r, &cur)
		if prev != cur {
			fmt.Println(prev, cur, p, last, i)
			partitions[p] = nodes[last:i]
			last = i
			p++
		}
		nodes[i].Partition = p
		prev = cur
	}

	var from, to int
	for i := 0; i < ne; i++ {
		fmt.Fscanln(r, &from, &to)

		edges[from] = append(edges[from], to)
		edges[to] = append(edges[to], from)
	}

	s.Graph = &Graph{nodes, partitions, edges}

	return
}
