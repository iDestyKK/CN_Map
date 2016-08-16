#CN_Map
A C library that implements C++ STL Maps as Red-Black Trees!

<b>NOTE: This is an experiment at best. Red-Black Trees are not functional at the moment.</b>

Maps are a fun data structure to use in C++. You have to specify two types to declare one. The first type is the key that the map uses as a sort of "identifier" for the second value, being the "value". In other words, the key "test" can return 0 if you wanted it to.

In C++
```c++
int main() {
	map<string, int> thing;
	thing.insert(pair<string, int>("test", 0));

	cout << thing["test"] << endl; //Prints out "0"
}
```

CN_Maps try to port that over with the same methods that I have used in my previous CN Data Structures.

```c
main() {
	CN_MAP map = cn_map_init(char*, int, cn_cmp_cstr);
	int a = 0;

	cn_map_insert(map, strdup("test"), &a);

	printf("%d\n", cn_map_find_deref(map, "test", int)); //Prints out "0"
	cn_free_map(map);
}
```

It's a work in progress. But it will have its functionality completed soon. :)
