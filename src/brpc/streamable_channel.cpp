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

#include <gflags/gflags.h>
#include <google/protobuf/descriptor.h>
#include "brpc/global.h"
#include "brpc/controller.h"

#include "brpc/streamable_channel.h"

namespace brpc {
namespace experimental {

int StreamableChannel::Init(const char* server_addr_and_port) {
    // GlobalInitializeOrDie();
    // _butex = bthread::butex_create_checked<base::atomic<int>>();
    // _butex->store(0);

    // base::EndPoint point;
    // if (str2endpoint(server_addr_and_port, &point) != 0 &&
    //     hostname2endpoint(server_addr_and_port, &point) != 0) {
    //     return -1;
    // }

    // _server_addr_and_port = server_addr_and_port;

    // SocketOptions sock_opt;
    // sock_opt.remote_side = point;
    // sock_opt.user = this;
    // sock_opt.on_edge_triggered_events = StreamChannel::OnNewMessage;

    // if (Socket::Create(sock_opt, &_sock_id) != 0) {
    //     STREAM_LOG(ERROR) << "Fail to socket create";
    //     return -1;
    // }

    // SocketUniquePtr sock;
    // if (Socket::Address(_sock_id, &sock) != 0) {
    //     STREAM_LOG(ERROR) << "Fail to address socket";
    //     return -1;
    // }
    // sock.get()->set_correlation_id(_wait_id.value);

    // // settings
    // base::IOBuf connect_meta;
    // connect_meta.append(_H2_CONNECTION_PREFACE_PREFIX);
    // char settingsbuf[58];
    // H2Settings settings;
    // settings.stream_window_size = 1 << 28;
    // settings.connection_window_size = 1 << 28;
    // const size_t nb = policy::SerializeH2SettingsFrameAndWU(
    //     settings, settingsbuf);
    // connect_meta.append(settingsbuf, nb);
    // if (!Write(_sock_id, &connect_meta)) {
    //     STREAM_LOG(ERROR) << "Fail to socket write";
    //     return -1;
    // }
    // // istio pings every 40s on average
    // const int64_t now_time_ms = base::gettimeofday_ms();
    // if (bthread_timer_add(&_timer_id,
    //                       base::milliseconds_to_timespec(now_time_ms + FLAGS_istio_keep_alive_timeout_ms),
    //                       &this->HandleTimeout, this) != 0) {
    //     STREAM_LOG(WARNING) << "Fail to reset timeout";
    // }
    LOG(INFO) << "StreamableChannel::Init";
    return 0;
}

void StreamableChannel::CallMethod(const google::protobuf::MethodDescriptor* method,
                                   google::protobuf::RpcController* controller,
                                   const google::protobuf::Message* request,
                                   google::protobuf::Message* response,
                                   google::protobuf::Closure* done) {                                
}

} // namespace experimental
} // namespace brpc