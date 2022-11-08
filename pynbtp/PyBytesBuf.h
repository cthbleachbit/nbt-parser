/**
 * @file python byte buffer wrapper
 *
 * Python seems to make a clear distinction between a str-like I/O object and a binary I/O object.
 * pybind11::detail::pythonbuf only handles plain-text I/O. So we are rolling our own for binary I/O.
 */

#ifndef NBTP_PYBYTESBUF_H
#define NBTP_PYBYTESBUF_H

#include <ostream>
#include <pybind11/pybind11.h>
#include <pybind11/pytypes.h>
#include <pybind11/cast.h>

namespace pyNBTP {
	/**
	 * @class A C++ usable output stream that writes to a binary I/O handle opened in python
	 */
	class PyOBytesBuf final : public std::streambuf {
	private:
		using traits_type = std::streambuf::traits_type;

		/**
		 * Buffer size - set in C-tor
		 */
		const size_t buf_size;

		/**
		 * Internal write buffer - allocated in C-tor
		 * Contents from C++ side will be buffered here and flushed out to python side as needed
		 */
		std::unique_ptr<char[]> d_buffer;

		/**
		 * Python binary output, write method
		 */
		pybind11::object pywrite;

		/**
		 * Python binary output, flush method
		 */
		pybind11::object pyflush;

	public:
		/**
		 * Executed when the buffer is full of contents
		 *
		 * Write them to python side and clear the buffer.
		 */
		int overflow(int c) final {
			if (!traits_type::eq_int_type(c, traits_type::eof())) {
				*pptr() = traits_type::to_char_type(c);
				pbump(1);
			}
			return sync() == 0 ? traits_type::not_eof(c) : traits_type::eof();
		}

		int sync() final {
			if (pbase() != pptr()) {
				pybind11::bytes line(pbase(), static_cast<size_t>(pptr() - pbase()));

				{
					pybind11::gil_scoped_acquire tmp;
					pywrite(line);
					pyflush();
				}

				setp(pbase(), epptr());
			}
			return 0;
		}

		explicit PyOBytesBuf(pybind11::object &pyostream, size_t buffer_size = 1024)
				: buf_size(buffer_size),
				  d_buffer(new char[buf_size]),
				  pywrite(pyostream.attr("write")),
				  pyflush(pyostream.attr("flush")) {
			setp(d_buffer.get(), d_buffer.get() + buf_size - 1);
		}

		PyOBytesBuf(PyOBytesBuf &&) = default;

		~PyOBytesBuf() final {
			sync();
		}
	};

	/**
	 * @class A C++ usable input stream that reads from a binary I/O handle opened in python
	 */
	class PyIBytesBuf final : public std::streambuf {
	private:
		using traits_type = std::streambuf::traits_type;

		/**
		 * Buffer size - set in C-tor
		 */
		const size_t buf_size;

		/**
		 * Internal read buffer - allocated in C-tor
		 */
		std::unique_ptr<char[]> d_buffer;

		/**
		 * Python input handle for reading
		 */
		pybind11::object pyread;

	public:
		/**
		 * Executed when the buffer is out of contents
		 *
		 * Read more from python side, write them to the buffer.
		 */
		int underflow() final {
			ssize_t actual_read = 0;
			if (!traits_type::eq_int_type(*gptr(), traits_type::eof())) {
				// Not EOF yet, try to read more stuff from pyistream
				ssize_t to_read = egptr() - gptr();
				if (to_read == 0) {
					// If we are at the end of the segment
					setg(d_buffer.get(), d_buffer.get(), d_buffer.get() + buf_size);
					to_read = buf_size;
				}

				pybind11::bytes b;
				{
					pybind11::gil_scoped_acquire tmp;
					b = pyread(to_read);
				}

				actual_read = pybind11::len(b);
				std::string incoming(b);
				const char *cstring = incoming.c_str();
				std::memcpy(d_buffer.get(), cstring, actual_read);
			}
			// fprintf(stderr, "Advancing buffer, len = %li\n", actual_read);
			int ret = actual_read == 0 ? traits_type::eof() : traits_type::not_eof(*gptr());
			return ret;
		}

		explicit PyIBytesBuf(pybind11::object &pyistream, size_t buffer_size = 1024)
				: buf_size(buffer_size),
				  d_buffer(new char[buf_size]),
				  pyread(pyistream.attr("read")) {
			setg(d_buffer.get(), d_buffer.get() + buf_size, d_buffer.get() + buf_size);
		};

		PyIBytesBuf(PyIBytesBuf &&) = default;
	};
}

#endif //NBTP_PYBYTESBUF_H
