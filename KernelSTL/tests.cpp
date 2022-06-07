#include "tests.hpp"
#include "tiny_stl.hpp"

#define Message(msg, ...) do {DbgPrintEx(0, 0, "[TinyTest]: " msg "\n", __VA_ARGS__);}while(0)
#define MessageOK(msg, ...) Message(msg " [OK]", __VA_ARGS__)
#define MessageFAILED(msg, ...) Message(msg " [FAILED]", __VA_ARGS__)
#define Execute(testName) \
if(testName()) \
	MessageOK(#testName); \
else \
	MessageFAILED(#testName)
#define UseCase(useCaseName) Message("    " useCaseName "...")
#define assert(cond) if(!(cond)) return false;

static bool testVector()
{
	UseCase("VectorDefaultConstructor");
	{
		tiny::vector<int> vec;

		assert(vec.empty());
		assert(vec.size() == 0);
		assert(vec.capacity() == 0);
		assert(vec.begin() == vec.end());
	}

	UseCase("VectorResizeConstructor");
	{
		tiny::vector<int> vec((size_t)5);

		assert(vec.empty() == false);
		assert(vec.size() == 5);
		assert(vec.capacity() == 5);
		assert(vec.begin() != vec.end());
	}

	// does not follow std::vector implementation due to variadic constructor
	// but it fills the missing std::initializer_list hole
	//UseCase("3");
	{
		//tiny::vector<int> vec(5);

		//assert(vec.empty() == false);
		//assert(vec.size() == 1);
		//assert(vec.capacity() >= 1);
		//assert(vec.front() == 5);
		//assert(vec.begin() != vec.end());
		//assert(*(vec.end()) == 5);
		//assert(*(vec.end()) == 5);
	}

	UseCase("VectorPushBack");
	{
		tiny::vector<int> vec;
		vec.push_back(5);

		assert(vec.empty() == false);
		assert(vec.size() == 1);
		assert(vec.capacity() >= 1);
		assert(vec.front() == 5);
		assert(vec.begin() != vec.end());
		assert(*(vec.begin()) == 5);
		assert(*(vec.data()) == 5);
	}

	UseCase("VectorClear");
	{
		tiny::vector<int> vec;
		vec.push_back(5);
		vec.clear();

		assert(vec.empty());
		assert(vec.size() == 0);
		assert(vec.capacity() >= 1);
		assert(vec.begin() == vec.end());
	}

	UseCase("VectorReserve");
	{
		tiny::vector<int> vec;
		vec.reserve(5);

		assert(vec.empty());
		assert(vec.size() == 0);
		assert(vec.capacity() == 5);
		assert(vec.begin() == vec.end());
	}

	UseCase("VectorResize");
	{
		tiny::vector<int> vec;
		vec.resize(5);

		assert(vec.empty() == false);
		assert(vec.size() == 5);
		assert(vec.capacity() == 5);
		assert(vec.begin() != vec.end());
	}

	UseCase("VectorResize");
	{
		tiny::vector<int> vec;
		vec.resize(10);
		vec.resize(5);

		assert(vec.empty() == false);
		assert(vec.size() == 5);
		assert(vec.capacity() == 10);
	}

	UseCase("VectorCapacityNotAffectedByResize");
	{
		tiny::vector<int> vec;
		vec.reserve(10);
		vec.resize(5);

		assert(vec.empty() == false);
		assert(vec.size() == 5);
		assert(vec.capacity() == 10);
	}

	UseCase("VectorCapacityNotAffectedByClear");
	{
		tiny::vector<int> vec;
		vec.resize(10);
		vec.clear();

		assert(vec.empty());
		assert(vec.size() == 0);
		assert(vec.capacity() == 10);
	}

	UseCase("VectorAssign");
	{
		tiny::vector<int> vec;
		vec.assign(10, 5);

		for (const auto& v : vec)
			assert(v == 5);

		assert(vec.empty() == false);
		assert(vec.size() == 10);
		assert(vec.capacity() == 10);
	}

	UseCase("VectorContinousMemory");
	{
		tiny::vector<int> vec;

		for (int i = 0; i < 100; i++)
			vec.push_back(i);

		for (int i = 0; i < 100; i++)
		{
			assert(vec[i] == i);
			assert(*(vec.data() + i) == i);
		}
	}

	UseCase("VectorShrinkToFit");
	{

		tiny::vector<int> vec;
		vec.reserve(10);
		vec.resize(5);

		assert(vec.size() == 5);
		assert(vec.capacity() == 10);

		vec.shrink_to_fit();

		assert(vec.size() == 5);
		assert(vec.capacity() == 5);

		vec.clear();
		vec.shrink_to_fit();

		assert(vec.empty());
		assert(vec.size() == 0);
		assert(vec.capacity() == 0);
	}

	return true;
}

static bool testString()
{
	UseCase("StringDefaultConstructor");
	{
		tiny::string str;

		assert(str.empty());
		assert(str.size() == 0);
		assert(str.capacity() == 0);
		assert(str.begin() == str.end());
	}

	UseCase("StringResizeConstructor");
	{
		tiny::string str((size_t)5);

		assert(str.empty() == false);
		assert(str.size() == 5);
		assert(str.capacity() == 5);
		assert(str.begin() != str.end());
	}

	UseCase("StringCorrectSize");
	{
		{
			tiny::string str;
			str = "hello";

			assert(str.empty() == false);
			assert(str.size() == 5);
			assert(str.capacity() == 5);
			assert(str.begin() != str.end());
		}

		{
			tiny::string str("hello");

			assert(str.empty() == false);
			assert(str.size() == 5);
			assert(str.size() == strlen(str.data()));
			assert(str.capacity() == 5);
			assert(str.begin() != str.end());
		}
	}

	UseCase("StringCompare");
	{
		{
			tiny::string str1("abc");
			tiny::string str2("abc");

			assert(str1.compare(str2) == 0);
			assert(str1.compare(str2) == strcmp(str1.data(), str2.data()));
		}

		{
			tiny::string str1("abc");
			tiny::string str2("abcd");

			assert(str1.compare(str2) == -1);
			assert(str1.compare(str2) == strcmp(str1.data(), str2.data()));
		}

		{
			tiny::string str1("abcd");
			tiny::string str2("abc");

			assert(str1.compare(str2) == 1);
			assert(str1.compare(str2) == strcmp(str1.data(), str2.data()));
		}

		{
			tiny::string str1("bbcd");
			tiny::string str2("abc");

			assert(str1.compare(str2) == 1);
			assert(str1.compare(str2) == strcmp(str1.data(), str2.data()));
		}

		{
			tiny::string str1("bbc");
			tiny::string str2("abcd");

			assert(str1.compare(str2) == 1);
			assert(str1.compare(str2) == strcmp(str1.data(), str2.data()));
		}
	}

	UseCase("StringFind");
	{
		{
			tiny::string str("");
			assert(str.find("") == tiny::string::npos);
			assert(str.find("a") == tiny::string::npos);
			assert(str.find('a') == tiny::string::npos);
		}

		{
			tiny::string str("abc");

			assert(str.find("") == tiny::string::npos);

			assert(str.find("a") == 0);
			assert(str.find('a') == 0);

			assert(str.find("b") == 1);
			assert(str.find('b') == 1);

			assert(str.find("c") == 2);
			assert(str.find('c') == 2);
		}

		{
			tiny::string str("abc");

			assert(str.find("abc") == 0);
			assert(str.find("bc") == 1);
		}

		{
			tiny::string str1("abcd");
			tiny::string str2("abc");

			assert(str1.find(str2) == 0);
			assert(str2.find(str1) == tiny::string::npos);
		}

		{
			tiny::string str1("abcdef");
			tiny::string str2("abc");

			assert(str1.find(str2) == 0);
			assert(str1.find(str2, 0) == 0);
			assert(str1.find(str2, 1) == tiny::string::npos);
		}
	}

	return true;
}

static bool testWstring()
{
	UseCase("WstringDefaultConstructor");
	{
		tiny::string str;

		assert(str.empty());
		assert(str.size() == 0);
		assert(str.capacity() == 0);
		assert(str.begin() == str.end());
	}

	UseCase("WstringResizeConstructor");
	{
		tiny::wstring str((size_t)5);

		assert(str.empty() == false);
		assert(str.size() == 5);
		assert(str.capacity() == 5);
		assert(str.begin() != str.end());
	}

	UseCase("WstringCorrectSize");
	{
		{
			tiny::wstring str;
			str = L"hello";

			assert(str.empty() == false);
			assert(str.size() == 5);
			assert(str.capacity() == 5);
			assert(str.begin() != str.end());
		}

		{
			tiny::wstring str(L"hello");

			assert(str.empty() == false);
			assert(str.size() == 5);
			assert(str.size() == wcslen(str.data()));
			assert(str.capacity() == 5);
			assert(str.begin() != str.end());
		}
	}

	UseCase("WstringCompare");
	{
		{
			tiny::wstring str1(L"abc");
			tiny::wstring str2(L"abc");

			assert(str1.compare(str2) == 0);
			assert(str1.compare(str2) == wcscmp(str1.data(), str2.data()));
		}

		{
			tiny::wstring str1(L"abc");
			tiny::wstring str2(L"abcd");

			assert(str1.compare(str2) == -1);
			assert(str1.compare(str2) == wcscmp(str1.data(), str2.data()));
		}

		{
			tiny::wstring str1(L"abcd");
			tiny::wstring str2(L"abc");

			assert(str1.compare(str2) == 1);
			assert(str1.compare(str2) == wcscmp(str1.data(), str2.data()));
		}

		{
			tiny::wstring str1(L"bbcd");
			tiny::wstring str2(L"abc");

			assert(str1.compare(str2) == 1);
			assert(str1.compare(str2) == wcscmp(str1.data(), str2.data()));
		}

		{
			tiny::wstring str1(L"cbc");
			tiny::wstring str2(L"abcd");

			assert(str1.compare(str2) == 1);
			assert(str1.compare(str2) == wcscmp(str1.data(), str2.data()));
		}
	}

	UseCase("WstringFind");
	{
		{
			tiny::wstring str(L"");
			assert(str.find(L"") == tiny::wstring::npos);
			assert(str.find(L"a") == tiny::wstring::npos);
			assert(str.find(L'a') == tiny::wstring::npos);
		}

		{
			tiny::wstring str(L"abc");

			assert(str.find(L"") == tiny::wstring::npos);

			assert(str.find(L"a") == 0);
			assert(str.find(L'a') == 0);

			assert(str.find(L"b") == 1);
			assert(str.find(L'b') == 1);

			assert(str.find(L"c") == 2);
			assert(str.find(L'c') == 2);
		}

		{
			tiny::wstring str(L"abc");

			assert(str.find(L"abc") == 0);
			assert(str.find(L"bc") == 1);
		}

		{
			tiny::wstring str1(L"abcd");
			tiny::wstring str2(L"abc");

			assert(str1.find(str2) == 0);
			assert(str2.find(str1) == tiny::wstring::npos);
		}

		{
			tiny::wstring str1(L"abcdef");
			tiny::wstring str2(L"abc");

			assert(str1.find(str2) == 0);
			assert(str1.find(str2, 0) == 0);
			assert(str1.find(str2, 1) == tiny::wstring::npos);
		}
	}

	return true;
}

namespace tiny {
	void runTests() {
		Message("Starting...");
		Execute(testVector);
		Execute(testString);
		Execute(testWstring);
		Message("Finished...");
	}
}