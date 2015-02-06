print("Hello World from Squirrel!\n")

function foo(e) {
	local c = e.createChild()
}

local a = Entity()
a.name = "AAA"
//a.sayHello()

local b = Entity()
b.name = "BBB"

foo(b)
print("after foo")

//b.sayHelloTo(a)

//c.sayHelloTo(a)


// function test() {
	
// 	// e.refCount == 1
// 	local e = Entity()

// 	e.name = "base"

// 	// e2.refCount == 2 (once in C++ as owned child of e, once in Squirrel as a local)
// 	local e2 = e.createChild()
// 	e2.name = "toto"

// 	// e gets released once, it is destroyed.
// 	// Its destructor releases e2 from C++, e2.refCounts == 1.
// 	// But...
// 	// e2 leaks? Why does Squirrel don't call the release hook?
// }

// test()
// print("After test")

// local m = MyObject()
// m.name = "toto"
// m.sayHello()




// class Hello extends Behaviour
// {
// 	function onReady() {
// 		print("Hello World!")
// 	}

// 	function onEvent(e) {
// 		switch(e.type) {
// 			case "keypressed":
// 				print("Key pressed! " + e.key.code)
// 			default:
// 				print("euh")
// 		}
// 	}
// }