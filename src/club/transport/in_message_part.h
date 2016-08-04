// Copyright 2016 Peter Jankuliak
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//     http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef CLUB_TRANSPORT_IN_MESSAGE_PART_H
#define CLUB_TRANSPORT_IN_MESSAGE_PART_H

#include <set>
#include <club/uuid.h>
#include "transport/sequence_number.h"
#include "transport/message_type.h"

#include <club/debug/ostream_uuid.h>
#include "debug/string_tools.h"

namespace club { namespace transport {

//------------------------------------------------------------------------------
struct InMessagePart {
  const uuid                      source;
        std::set<uuid>            targets;
  const MessageType               type;
  const SequenceNumber            sequence_number;
        size_t                    original_size;
        size_t                    chunk_start;
        size_t                    chunk_size;
        boost::asio::const_buffer payload;
        boost::asio::const_buffer type_and_payload;

  bool is_full() const;

  InMessagePart( uuid                      source
               , std::set<uuid>&&          targets
               , MessageType               type
               , SequenceNumber            sequence_number
               , size_t                    original_size
               , size_t                    chunk_start
               , size_t                    chunk_size
               , boost::asio::const_buffer payload
               , boost::asio::const_buffer type_and_payload)
    : source(std::move(source))
    , targets(std::move(targets))
    , type(type)
    , sequence_number(sequence_number)
    , original_size(original_size)
    , chunk_start(chunk_start)
    , chunk_size(chunk_size)
    , payload(payload)
    , type_and_payload(type_and_payload)
  {}
};

//------------------------------------------------------------------------------
inline
bool InMessagePart::is_full() const {
  return chunk_start == 0 && original_size == chunk_size;
}

//------------------------------------------------------------------------------
inline std::ostream& operator<<(std::ostream& os, const InMessagePart& m) {
  using namespace boost::asio;

  os << "(InMessagePart src:" << m.source
     << " targets:" << str(m.targets)
     << " type:" << m.type
     << " sn:" << m.sequence_number
     << " orig_size:" << m.original_size
     << " chunk_start:" << m.chunk_start
     << " chunk_size:" << m.chunk_size;

  const_buffer b( buffer_cast<const uint8_t*>(m.payload)
                , std::min<size_t>(10, buffer_size(m.payload)));

  os << " " << str(b) << "x" << buffer_size(m.payload)
     << ")";

  return os;
}

}} // club::transport namespace

#endif // ifndef CLUB_TRANSPORT_IN_MESSAGE_PART_H
