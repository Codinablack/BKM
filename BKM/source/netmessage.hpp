#ifndef _NETMESSAGE_HPP
#define _NETMESSAGE_HPP


#include "console.hpp"

namespace BKM {

	/// <summary>
	/// MessageHeader is at the beginning of all messages.
	/// We use template here to allow us to use enum classes for the identifier.
	/// </summary>
	/// <typeparam name="T"></typeparam>
	template <typename T>
	struct MessageHeader {
		T id{};
		uint32_t size = 0;
	};

	template <typename T>
	struct Message {
		MessageHeader<T> header{};
		std::vector<uint8_t> body;

		// get size of entire packet
		size_t size() const {
			return sizeof( MessageHeader<T> ) + body.size();
		}

		// we overload the << operator to allow compatibility with console::print, std::cout and other ostreams.
		friend ostream& operator << ( std::ostream outstream, const Message<T>& msg ) {
			outstream << "Packet ID: " << int( msg.header.id ) << " || Size: " << msg.size();
			return outstream;
		}

		// Pushes any POD-like data into message.
		template <typename DataType>
		friend Message<T>& operator << ( Message<T>&msg, const DataType & data ) {

			// first we check to make sure its usable data type.
			static_assert( std::is_standard_layout<DataType>::value, "Data is too complex" );

			// serves as entry point for accessing memory location
			size_t cache = msg.body.size();

			// need to resize because we are adding data.
			// resize by adding size of DataType
			msg.body.resize( msg.body.size() + sizeof(DataType) );

			// now we have accounted for the extra size, lets add the data
			// memcopy( destination, sourceData, size )
			std::memcpy( msg.body.data() + cache, &data, sizeof(DataType) );

			// update the message header size to reflect changes
			msg.header.size = msg.size();

			// always return msg so we can << chain
			return msg;
		}

		// Can be used for receiving POD-like data.
		template <typename DataType>
		friend Message<T>& operator >> ( Message<T>& msg, const DataType& data ) {

			// first we check to make sure its usable data type
			static_assert( std::is_standard_layout<DataType>::value, "Data is too complex" );

			// serves as the location in memory we will pull data from
			size_t cache = msg.size() - sizeof(DataType);


			// now we have accounted for size and memory location, lets pull the data
			// memcopy( destination, sourceData, size )
			std::memcpy( &data, msg.body.data() + cache, sizeof(DataType) );

			// shrink body, remove read bytes, reset end position
			msg.body.resize(cache);

			// update size variable
			msg.header.size = msg.size();

			// always return msg so we can << chain
			return msg;
		}
	};
}

#endif // _NETMESSAGE_HPP