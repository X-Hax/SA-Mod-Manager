/**
 * SADX Mod Loader.
 * Memory access inline functions.
 */

#ifndef MODLOADER_MEMACCESS_H
#define MODLOADER_MEMACCESS_H

#include <stdint.h>
#include <iterator>
#include <stdexcept>

// Utility Functions

/**
* Get the number of elements in an array.
* @return Number of elements in the array.
*/
template <typename Tret = size_t, typename T, size_t N>
static constexpr Tret LengthOfArray(const T(&)[N]) noexcept
{
	return (Tret)N;
}

/**
* Get the size of an array.
* @return Size of the array, in bytes.
*/
template <typename Tret = size_t, typename T, size_t N>
static constexpr Tret SizeOfArray(const T(&)[N]) noexcept
{
	return (Tret)(N * sizeof(T));
}

// Macros for functions that need both an array
// and the array length or size.
#define arrayptrandlengthT(data,T) data, LengthOfArray<T>(data)
#define arraylengthandptrT(data,T) LengthOfArray<T>(data), data
#define arrayptrandsizeT(data,T) data, SizeOfArray<T>(data)
#define arraysizeandptrT(data,T) SizeOfArray<T>(data), data

// Macros for functions that need both an array
// and the array length or size.
#define arrayptrandlength(data) data, LengthOfArray(data)
#define arraylengthandptr(data) LengthOfArray(data), data
#define arrayptrandsize(data) data, SizeOfArray(data)
#define arraysizeandptr(data) SizeOfArray(data), data

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>

static HANDLE curproc;
static bool curprocinitialized = false;

static inline BOOL WriteData(void *writeaddress, const void *data, SIZE_T datasize, SIZE_T *byteswritten)
{
	if (!curprocinitialized)
	{
		curproc = GetCurrentProcess();
		curprocinitialized = true;
	}
	return WriteProcessMemory(curproc, writeaddress, data, datasize, byteswritten);
}

static inline BOOL WriteData(void *writeaddress, const void *data, SIZE_T datasize)
{
	return WriteData(writeaddress, data, datasize, nullptr);
}

template<typename T>
static inline BOOL WriteData(T const *writeaddress, const T data, SIZE_T *byteswritten)
{
	return WriteData((void*)writeaddress, (void*)&data, (SIZE_T)sizeof(data), byteswritten);
}

template<typename T>
static inline BOOL WriteData(T const *writeaddress, const T data)
{
	return WriteData(writeaddress, data, nullptr);
}

template<typename T>
static inline BOOL WriteData(T *writeaddress, const T &data, SIZE_T *byteswritten)
{
	return WriteData(writeaddress, &data, sizeof(data), byteswritten);
}

template<typename T>
static inline BOOL WriteData(T *writeaddress, const T &data)
{
	return WriteData(writeaddress, data, nullptr);
}

template <typename T, size_t N>
static inline BOOL WriteData(void *writeaddress, const T(&data)[N], SIZE_T *byteswritten)
{
	return WriteData(writeaddress, data, SizeOfArray(data), byteswritten);
}

template <typename T, size_t N>
static inline BOOL WriteData(void *writeaddress, const T(&data)[N])
{
	return WriteData(writeaddress, data, nullptr);
}

/**
 * Write a repeated byte to an arbitrary address.
 * @param address	[in] Address.
 * @param data		[in] Byte to write.
 * @param byteswritten	[out, opt] Number of bytes written.
 * @return Nonzero on success; 0 on error (check GetLastError()).
 */
template <SIZE_T count>
static inline BOOL WriteData(void *address, uint8_t data, SIZE_T *byteswritten)
{
	uint8_t buf[count];
	memset(buf, data, count);
	int result = WriteData(address, buf, count, byteswritten);
	return result;
}

/**
 * Write a repeated byte to an arbitrary address.
 * @param address	[in] Address.
 * @param data		[in] Byte to write.
 * @return Nonzero on success; 0 on error (check GetLastError()).
 */
template <SIZE_T count>
static inline BOOL WriteData(void *address, uint8_t data)
{
	return WriteData<count>(address, data, nullptr);
}

#if (defined(__i386__) || defined(_M_IX86)) && \
	!(defined(__x86_64__) || defined(_M_X64))

// JMP/CALL DWORD relative opcode union.
#pragma pack(1)
union JmpCallDwordRel {
	struct {
		uint8_t opcode;
		int32_t address;
	};
	uint8_t u8[5];

	JmpCallDwordRel() {}

	JmpCallDwordRel(bool isCall, intptr_t src, intptr_t dst)
	{
		opcode = isCall ? 0xE8 : 0xE9;
		address = dst - (src + 5);
	}

	JmpCallDwordRel(bool isCall, void* src, void* dst)
	{
		opcode = isCall ? 0xE8 : 0xE9;
		address = (intptr_t)dst - ((intptr_t)src + 5);
	}
};
#pragma pack()

/**
 * Write a JMP instruction to an arbitrary address.
 * @param writeaddress Address to insert the JMP instruction.
 * @param funcaddress Address to JMP to.
 * @return Nonzero on success; 0 on error (check GetLastError()).
 */
static inline BOOL WriteJump(void *writeaddress, void *funcaddress)
{
	JmpCallDwordRel data;
	data.opcode = 0xE9; // JMP DWORD (relative)
	data.address = static_cast<int32_t>((intptr_t)funcaddress - ((intptr_t)writeaddress + 5));
	return WriteData(writeaddress, data.u8);
}

/**
 * Write a CALL instruction to an arbitrary address.
 * @param writeaddress Address to insert the CALL instruction.
 * @param funcaddress Address to CALL.
 * @return Nonzero on success; 0 on error (check GetLastError()).
 */
static inline BOOL WriteCall(void *writeaddress, void *funcaddress)
{
	JmpCallDwordRel data;
	data.opcode = 0xE8;	// CALL DWORD (relative)
	data.address = static_cast<int32_t>((intptr_t)funcaddress - ((intptr_t)writeaddress + 5));
	return WriteData(writeaddress, data.u8);
}

#endif

// Data pointer and array declarations.
#define DataPointer(type, name, address) \
	static type &name = *(type *)address
#define DataArray(type, name, address, len) \
	static DataArray_t<type, address, len> name

template<typename T, intptr_t addr, size_t len>
struct DataArray_t final
{
	typedef T value_type;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;
	typedef value_type& reference;
	typedef const value_type& const_reference;
	typedef value_type* pointer;
	typedef const value_type* const_pointer;
	typedef pointer iterator;
	typedef const_pointer const_iterator;
	typedef std::reverse_iterator<iterator> reverse_iterator;
	typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

	DataArray_t() = default; // have to declare default constructor
	DataArray_t(const DataArray_t&) = delete; // object cannot be copied, prevents accidentally using DataArray in a function call
	DataArray_t(const DataArray_t&&) = delete; // object cannot be moved

	// Gets the underlying data for the array.
	constexpr pointer data() const noexcept { return reinterpret_cast<pointer>(addr); }
	// Gets the underlying data for the array.
	constexpr const_pointer cdata() const noexcept { return reinterpret_cast<const_pointer>(addr); }

	// Checks if the array is empty (no elements).
	constexpr bool empty() const noexcept { return len == 0; }

	// Gets the size of the array, in elements.
	constexpr size_type size() const noexcept { return len; }

	// Gets the maximum size of the array, in elements.
	constexpr size_type max_size() const noexcept { return len; }

	constexpr pointer operator&() const noexcept { return data(); }

	constexpr operator pointer() const noexcept { return data(); }

	// Gets an item from the array, with bounds checking.
	constexpr reference at(size_type i)
	{
		if (i < len)
			return data()[i];
		throw std::out_of_range("Data access out of range.");
	}

	// Gets an item from the array, with bounds checking.
	constexpr const_reference at(size_type i) const
	{
		if (i < len)
			return cdata()[i];
		throw std::out_of_range("Data access out of range.");
	}

	template<size_type I>
	// Gets an item from the array, with compile-time bounds checking.
	constexpr reference get() noexcept
	{
		static_assert(I < len, "index is within bounds");
		return data()[I];
	}

	template<size_type I>
	// Gets an item from the array, with compile-time bounds checking.
	constexpr const_reference get() const noexcept
	{
		static_assert(I < len, "index is within bounds");
		return cdata()[I];
	}

	// Gets the first item in the array.
	constexpr reference front() { return *data(); }
	// Gets the first item in the array.
	constexpr const_reference front() const { return *cdata(); }

	// Gets the last item in the array.
	constexpr reference back() { return data()[len - 1]; }
	// Gets the last item in the array.
	constexpr const_reference back() const { return cdata()[len - 1]; }

	// Gets an iterator to the beginning of the array.
	constexpr iterator begin() noexcept { return data(); }
	// Gets an iterator to the beginning of the array.
	constexpr const_iterator begin() const noexcept { return cdata(); }
	// Gets an iterator to the beginning of the array.
	constexpr const_iterator cbegin() const noexcept { return cdata(); }

	// Gets an iterator to the end of the array.
	constexpr iterator end() noexcept { return data() + len; }
	// Gets an iterator to the end of the array.
	constexpr const_iterator end() const noexcept { return cdata() + len; }
	// Gets an iterator to the end of the array.
	constexpr const_iterator cend() const noexcept { return cdata() + len; }

	// Gets a reverse iterator to the beginning of the array.
	constexpr reverse_iterator rbegin() noexcept { return data() + len; }
	// Gets a reverse iterator to the beginning of the array.
	constexpr const_reverse_iterator rbegin() const noexcept { return cdata() + len; }
	// Gets a reverse iterator to the beginning of the array.
	constexpr const_reverse_iterator crbegin() const noexcept { return cdata() + len; }

	// Gets a reverse iterator to the end of the array.
	constexpr reverse_iterator rend() noexcept { return data(); }
	// Gets a reverse iterator to the end of the array.
	constexpr const_reverse_iterator rend() const noexcept { return cdata(); }
	// Gets a reverse iterator to the end of the array.
	constexpr const_reverse_iterator crend() const noexcept { return cdata(); }
};

// Function pointer declarations.
#define FunctionPointer(RETURN_TYPE, NAME, ARGS, ADDRESS) \
	static RETURN_TYPE (__cdecl *const NAME)ARGS = (RETURN_TYPE (__cdecl *)ARGS)ADDRESS
#define StdcallFunctionPointer(RETURN_TYPE, NAME, ARGS, ADDRESS) \
	static RETURN_TYPE (__stdcall *const NAME)ARGS = (RETURN_TYPE (__stdcall *)ARGS)ADDRESS
#define FastcallFunctionPointer(RETURN_TYPE, NAME, ARGS, ADDRESS) \
	static RETURN_TYPE (__fastcall *const NAME)ARGS = (RETURN_TYPE (__fastcall *)ARGS)ADDRESS
#define ThiscallFunctionPointer(RETURN_TYPE, NAME, ARGS, ADDRESS) \
	static RETURN_TYPE (__thiscall *const NAME)ARGS = (RETURN_TYPE (__thiscall *)ARGS)ADDRESS
#define VoidFunc(NAME, ADDRESS) FunctionPointer(void,NAME,(void),ADDRESS)

// Non-static FunctionPointer.
// If declaring a FunctionPointer within a function, use this one instead.
// Otherwise, the program will crash on Windows XP.
#define NonStaticFunctionPointer(RETURN_TYPE, NAME, ARGS, ADDRESS) \
	RETURN_TYPE (__cdecl *const NAME)ARGS = (RETURN_TYPE (__cdecl *)ARGS)ADDRESS

#define patchdecl(address,data) { (void*)address, arrayptrandsize(data) }
#define ptrdecl(address,data) { (void*)address, (void*)data }

#endif /* MODLOADER_MEMACCESS_H */
