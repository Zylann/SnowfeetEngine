Coding style
=============

File naming
------------

Files defining one or more classes take the name of the main class, with the same case.
If the class is an interface, don't prefix with "I".

If they don't contain classes, use lower case and underscores.

C++ headers have .hpp extension.
C++ files have .cpp extension.


Encoding
---------

Use UTF-8, with CRLF.
Indent with 4 spaces. Never use tabs.


Coding style sample
---------------------

	namespace ns
	{

	enum MyEnum
	{
		NS_ME_AAA = 1,
		NS_ME_BBB = 2
		//...
	};

	/// \brief Description
	class MyClass : public Base, public IInterface
	{
	public:
		MyClass():
			publicAttrib(0),
			m_attrib(42),
			m_ownedAttrib(nullptr),
			r_referencedAttrib(nullptr)
		{}

		~MyClass()
		{
			if(m_ownedAttrib)
				delete m_ownedAttrib;
		}

		void foo() {}
		inline int getter() const { return m_attrib; }

		int publicAttrib;

		/// \brief Description
		/// \param x: Test variable
		/// \return stuff
		bool test(float x)
		{
			Vector4 someArray[4] = {
				{1, 0, 0, 0},
				{0, 1, 0, 0},
				{0, 0, 1, 0},
				{0, 0, 0, 1}
			};

			callBigFunction(
				"param1",
				"param2",
				"param3",
				"param4"
			);

	#ifdef DEFINED_X
			// Do special stuff
	#endif

			return true;
		}

		void callBigFunction(
			const std::string & first,
			const std::string & second,
			const std::string & third,
			const std::string & fourth
		);

	private:
		int m_attrib;
		int * m_ownedAttrib;
		int * r_referencedAttrib;

	}

	} // namespace ns


Coding style notes
-------------------

Don't abuse code blocks without braces.

Try to not cross the 100 characters margin, use the presentation
guides from the sample code above, if possible.

Don't abuse long type names, especially with STL templates:
use typedef or auto to simplify reading and maintainability.

Visual Studio 2013 forces indenting of namespaces.
It wastes 4 spaces on the beginning of each line,
so don't let him do that, and unindent.

Don't abuse #define unless you have no choice.
C++ constants, enums and inlines are often a more elegant option.

Don't #ifdef PLATFORM. It's handled by premake already.
Platform-specific implementations must be in separate files, not a cluttered one.

NEVER DO using namespace std.

