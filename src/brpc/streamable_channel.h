// Licensed to the Apache Software Foundation (ASF) under one
// or more contributor license agreements.  See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  The ASF licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.


#ifndef BRPC_STREAMABLE_CHANNEL_H
#define BRPC_STREAMABLE_CHANNEL_H

// To brpc developers: This is a header included by user, don't depend
// on internal structures, use opaque pointers instead.

#include "brpc/shared_object.h"
#include "brpc/channel.h"

namespace brpc {
namespace experimental {

class StreamableChannel : public ChannelBase {
friend class Controller;
public:
    StreamableChannel() {}

    int Init(const char* server_addr_and_port);

    void CallMethod(const google::protobuf::MethodDescriptor* method,
                    google::protobuf::RpcController* controller,
                    const google::protobuf::Message* request,
                    google::protobuf::Message* response,
                    google::protobuf::Closure* done) override;

    int Weight() { return 1; }

protected:

    int CheckHealth() override { return 0; }
};

} // namespace experimental
} // namespace brpc


#endif  // BRPC_STREAMABLE_CHANNEL_H
