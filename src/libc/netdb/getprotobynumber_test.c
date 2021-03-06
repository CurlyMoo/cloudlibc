// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <netinet/in.h>

#include <netdb.h>
#include <stddef.h>
#include <testing.h>

TEST(getprotobynumber, bad) {
  ASSERT_EQ(NULL, getprotobynumber(-1));
  ASSERT_EQ(NULL, getprotobynumber(180));
  ASSERT_EQ(NULL, getprotobynumber(256));
}

TEST(getprotobynumber, ipproto_constants) {
  ASSERT_EQ(IPPROTO_ICMP, getprotobynumber(IPPROTO_ICMP)->p_proto);
  ASSERT_EQ(IPPROTO_IP, getprotobynumber(IPPROTO_IP)->p_proto);
  ASSERT_EQ(IPPROTO_IPV6, getprotobynumber(IPPROTO_IPV6)->p_proto);
  ASSERT_EQ(IPPROTO_RAW, getprotobynumber(IPPROTO_RAW)->p_proto);
  ASSERT_EQ(IPPROTO_TCP, getprotobynumber(IPPROTO_TCP)->p_proto);
  ASSERT_EQ(IPPROTO_UDP, getprotobynumber(IPPROTO_UDP)->p_proto);
}
