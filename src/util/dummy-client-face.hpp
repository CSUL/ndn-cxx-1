/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/**
 * Copyright (c) 2013-2015 Regents of the University of California.
 *
 * This file is part of ndn-cxx library (NDN C++ library with eXperimental eXtensions).
 *
 * ndn-cxx library is free software: you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later version.
 *
 * ndn-cxx library is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more details.
 *
 * You should have received copies of the GNU General Public License and GNU Lesser
 * General Public License along with ndn-cxx, e.g., in COPYING.md file.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * See AUTHORS.md for complete list of ndn-cxx authors and contributors.
 */

#ifndef NDN_UTIL_DUMMY_CLIENT_FACE_HPP
#define NDN_UTIL_DUMMY_CLIENT_FACE_HPP

#include "../face.hpp"
#include "signal.hpp"
#include "../lp/packet.hpp"

#define NDN_UTIL_DUMMY_FACE_KEEP_DEPRECATED

namespace ndn {
namespace util {

/** \brief a client-side face for unit testing
 */
class DummyClientFace : public ndn::Face
{
public:
  /** \brief options for DummyClientFace
   */
  struct Options
  {
    /** \brief if true, packets sent out of DummyClientFace will be appended to a container
     */
    bool enablePacketLogging;

    /** \brief if true, prefix registration command will be automatically
     *         replied with a successful response
     */
    bool enableRegistrationReply;
  };

  /**
   * @brief Create a dummy face with internal IO service
   */
  DummyClientFace(const Options& options = DummyClientFace::DEFAULT_OPTIONS);

  /**
   * @brief Create a dummy face with the provided IO service
   */
  DummyClientFace(boost::asio::io_service& ioService, const Options& options = DummyClientFace::DEFAULT_OPTIONS);

  /** \brief cause the Face to receive a packet
   *  \tparam Packet either Interest or Data
   */
  template<typename Packet>
  void
  receive(const Packet& packet);

private: // constructors
  class Transport;

  void
  construct(const Options& options);

private:
  void
  enablePacketLogging();

  void
  enableRegistrationReply();

public:
  /** \brief default options
   *
   *  enablePacketLogging=true
   *  enableRegistrationReply=false
   */
  static const Options DEFAULT_OPTIONS;

  /** \brief Interests sent out of this DummyClientFace
   *
   *  Sent Interests are appended to this container if options.enablePacketLogger is true.
   *  User of this class is responsible for cleaning up the container, if necessary.
   *  After .expressInterest, .processEvents must be called before the Interest would show up here.
   */
  std::vector<Interest> sentInterests;

  /** \brief Data sent out of this DummyClientFace
   *
   *  Sent Data are appended to this container if options.enablePacketLogger is true.
   *  User of this class is responsible for cleaning up the container, if necessary.
   *  After .put, .processEvents must be called before the Data would show up here.
   */
  std::vector<Data> sentData;

#ifdef NDN_UTIL_DUMMY_FACE_KEEP_DEPRECATED
  std::vector<Data>& sentDatas; ///< deprecated alias to sentData
#endif // NDN_UTIL_DUMMY_FACE_KEEP_DEPRECATED

  /** \brief NACKs sent out of this DummyClientFace
   *
   *  Sent NACKs are appended to this container if options.enablePacketLogger is true.
   *  User of this class is responsible for cleaning up the container, if necessary.
   *  After .put, .processEvents must be called before the NACK would show up here.
   */
  std::vector<lp::Nack> sentNacks;

  /** \brief emits whenever an Interest is sent
   *
   *  After .expressInterest, .processEvents must be called before this signal would be emitted.
   */
  Signal<DummyClientFace, Interest> onSendInterest;

  /** \brief emits whenever a Data packet is sent
   *
   *  After .put, .processEvents must be called before this signal would be emitted.
   */
  Signal<DummyClientFace, Data> onSendData;

  /** \brief emits whenever a NACK is sent
   *
   *  After .put, .processEvents must be called before this signal would be emitted.
   */
  Signal<DummyClientFace, lp::Nack> onSendNack;
};

template<>
void
DummyClientFace::receive(const lp::Nack& nack);

#ifdef NDN_UTIL_DUMMY_FACE_KEEP_DEPRECATED
/**
 * @brief Create a dummy face with internal IO service
 * @deprecated Use the DummyFace constructor directly
 */
shared_ptr<DummyClientFace>
makeDummyClientFace(const DummyClientFace::Options& options = DummyClientFace::DEFAULT_OPTIONS);

/**
 * @brief Create a dummy face with the provided IO service
 * @deprecated Use the DummyFace constructor directly
 */
shared_ptr<DummyClientFace>
makeDummyClientFace(boost::asio::io_service& ioService,
                    const DummyClientFace::Options& options = DummyClientFace::DEFAULT_OPTIONS);
#endif // NDN_UTIL_DUMMY_FACE_KEEP_DEPRECATED

} // namespace util
} // namespace ndn

#endif // NDN_UTIL_DUMMY_CLIENT_FACE_HPP
