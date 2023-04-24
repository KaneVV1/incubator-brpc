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

#ifndef BRPC_STREAM_INL_H
#define BRPC_STREAM_INL_H

#include "butil/logging.h"

namespace brpc {
namespace experimental {

template <typename W, typename R, typename Func, typename Stub>
inline RpcExecutor<W, R, Func, Stub>::RpcExecutor(RpcExecutor&& other) 
    : _sm(std::move(other._sm)), _stub(other._stub), _cntl(other._cntl), _running(other._running) {
        other._stub = nullptr;
        other._cntl = nullptr;
        other._running = false;
}

namespace grpcvisit {

template <typename Executor>
inline bool GrpcReaderWriter<Executor>::Start() {
	return true;
}

template <typename Executor>
inline void GrpcReaderWriter<Executor>::Finish() {
}

template <typename Executor>
inline bool GrpcReaderWriter<Executor>::Read(RT* msg) {
	return true;
}

template <typename Executor>
inline bool GrpcReaderWriter<Executor>::Read(RT* msg, int32_t timeout_ms) {
	return true;
}

template <typename Executor>
inline bool GrpcReaderWriter<Executor>::Write(const WT& msg) {
	return true;
}

template <typename Executor>
inline bool GrpcReaderWriter<Executor>::WritesDone() {
	return true;
}

template <typename Executor>
inline bool GrpcReader<Executor>::Start() {
	return true;
}

template <typename Executor>
inline void GrpcReader<Executor>::Finish() {
}

template <typename Executor>
inline bool GrpcReader<Executor>::Read(RT* msg) {
	return true;
}

template <typename Executor>
inline bool GrpcReader<Executor>::Read(RT* msg, int32_t timeout_ms) {
	return true;
}

template <typename Executor>
inline bool GrpcWriter<Executor>::Start() {
	return true;
}

template <typename Executor>
inline void GrpcWriter<Executor>::Finish() {
}

template <typename Executor>
inline bool GrpcWriter<Executor>::Write(const WT& msg) {
	return true;
}

template <typename Executor>
inline bool GrpcWriter<Executor>::WritesDone() {
	return true;
}

template <typename Executor>
inline bool GrpcCaller<Executor>::Call() {
	
	return true;
}

} // namespace grpcvisit

} // namespace experimental
} // namespace brpc

#endif // BRPC_STREAM_INL_H