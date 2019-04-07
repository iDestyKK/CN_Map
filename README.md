# CN_Map

## Synopsis
A C library that implements C++ STL Maps as Red-Black Trees!

Maps are a fun data structure to use in C++. You have to specify two types to declare one. The first type is the key that the map uses as a sort of "identifier" for the second value, being the "value". In other words, the key "test" can return 0 if you wanted it to.

## Documentation
Full documentation is available at: http://docs.claranguyen.me/lib.php?id=cnds/cn_map

## Example
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
	//Create a CN_Map with char*'s as keys, ints as values, and a valid
	//comparison function for C strings
	CN_MAP map = cn_map_init(char *, int, cn_cmp_cstr);

	//Create temporary variables to insert
	char *key   = "hello";
	int   value = 0;

	//Duplicate the key (This is for char*'s only)
	char *dup = strdup(key);

	//Insert into the CN_Map
	cn_map_insert(map, &dup, &value);

	//Find the key/value pair by its key
	CNM_ITERATOR it;
	cn_map_find(map, &it, &key);

	//Print out the iterator's key
	printf("%d\n", cn_map_iterator_value(&it, int)); //Prints out "0"

	//Free memory
	cn_map_free(map);
}
```
