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

#ifndef BRPC_GRPC_STREAM_H
#define BRPC_GRPC_STREAM_H

#include <type_traits>
#include <memory>

#include "brpc/controller.h"

namespace brpc {
namespace experimental {

template <typename R>
class ReaderInterface {
public:
    virtual ~ReaderInterface() {}
    virtual bool Read(R* msg) = 0;
    virtual bool Read(R* msg, int32_t timeout_ms) = 0;
};

template <typename W>
class WriterInterface {
public:
    virtual ~WriterInterface() {}
    virtual bool Write(const W& msg) = 0;
    virtual bool WritesDone() = 0;
};

class StreamingInterface {
public:
    virtual ~StreamingInterface() {}
    virtual bool Start() = 0;
    virtual void Finish() = 0;
};

class CallInterface {
public:
    virtual ~CallInterface() {}
    virtual bool Call() = 0;
};

// struct Codec {

//     template <typename... Ts>
//     struct Encode;

//     template <typename T>
//     struct Encode<T> {
//         void operator()(Controller* cntl) const {
//             T(cntl);
//         }
//     };

//     template <typename T1, typename... Ts>
//     struct Encode<T1, Ts...> {
//         void operator()(Controller* cntl) const {
//             Encode<T1>(cntl);
//             if (!cntl->Failed()) {
//                 Encode<Ts...>(cntl);
//             }
//         }
//     };
// };

template <typename W, typename R, typename Func, typename Stub>
class RpcExecutor {
public:
    using ResponseType = R;
    using RequestType = W;

    RpcExecutor(Func&& func, Stub* stub, Controller* cntl)
        : _sm(std::move(func)), _stub(stub), _cntl(cntl) {}

    RpcExecutor(RpcExecutor&& other);

    void Run() {
		LOG(INFO) << "RpcExecutor::Run()";
    }

private:
    Func _sm;
    Stub* _stub;
    Controller* _cntl;

    bool _running = false;
};

template <typename W, typename R, typename Func, typename Stub>
inline RpcExecutor<W, R, Func, Stub> MakeRpcExecutor(Func&& func, Stub* stub, Controller* cntl) {
    return RpcExecutor<W, R, Func, Stub>(std::forward<Func>(func), stub, cntl);
}

template <typename V>
static inline typename std::enable_if<
	std::is_base_of<StreamingInterface, V>::value, bool>::type
StartIfPossible(V* ptr) { return ptr->Start(); }

template <typename V>
inline typename std::enable_if<
	!std::is_base_of<StreamingInterface, V>::value, bool>::type
StartIfPossible(V* ptr) { return true; }

template <typename V>
inline typename std::enable_if<
	std::is_base_of<StreamingInterface, V>::value, void>::type
FinishIfPossibleAndDelete(V* ptr) { ptr->Finish(); delete ptr; }

template <typename V>
inline typename std::enable_if<
	!std::is_base_of<StreamingInterface, V>::value, void>::type
FinishIfPossibleAndDelete(V* ptr) { delete ptr; }

template <typename V>
struct Deleter {
	void operator()(V* ptr) { FinishIfPossibleAndDelete(ptr); }
};

template <typename V, typename... Args>
inline std::unique_ptr<V, Deleter<V>> MakeStreamableVisit(Args&&... args) {
	auto pv = new (std::nothrow) V(std::forward<Args>(args)...);
	if (!pv) { return nullptr; }
	if (!StartIfPossible(pv)) {
		delete pv;
		return nullptr;
	}
	return std::unique_ptr<V, Deleter<V>>(pv);
}

template <template <typename> class Visit, typename... Args>
using RpcVisit = Visit<RpcExecutor<Args...>>;

template <template <typename> class Visit, typename... Args>
using RpcVisitUniquePtr = std::unique_ptr<
	RpcVisit<Visit, Args...>, Deleter<RpcVisit<Visit, Args...>>>;


// TODO: text
namespace grpcvisit {

template <typename Executor>
class GrpcReaderWriter final : public StreamingInterface,
                               public ReaderInterface<typename Executor::ResponseType>,
                               public WriterInterface<typename Executor::RequestType> {
public:
    using RT = typename Executor::ResponseType;
    using WT = typename Executor::RequestType;

    GrpcReaderWriter() = delete;

    explicit GrpcReaderWriter(Executor&& executor) : _executor(std::move(executor)) {}

    bool Start() override;
    void Finish() override;
    bool Read(RT* msg) override;
    bool Read(RT* msg, int32_t timeout_ms) override;
    bool Write(const WT& msg) override;
    bool WritesDone() override;

private:
    Executor _executor;
};

template <typename Executor>
class GrpcReader final : public StreamingInterface,
                         public ReaderInterface<typename Executor::ResponseType> {
public:
    using RT = typename Executor::ResponseType;
    using WT = typename Executor::RequestType;

    GrpcReader() = delete;
    GrpcReader(Executor&& executor, const WT& req) 
        : _executor(std::move(executor)), _req(req) {}

    bool Start() override;
    void Finish() override;
    bool Read(RT* msg) override;
    bool Read(RT* msg, int32_t timeout_ms) override;

private:
    Executor _executor;
    const WT& _req;
};

template <typename Executor>
class GrpcWriter final : public StreamingInterface,
                         public WriterInterface<typename Executor::RequestType> {
public:
    using RT = typename Executor::ResponseType;
    using WT = typename Executor::RequestType;

    GrpcWriter() = delete;
    GrpcWriter(Executor&& executor, RT* res) 
        : _executor(std::move(executor)), _res(res) {}

    bool Start() override;
    void Finish() override;
    bool Write(const WT& msg) override;
    bool WritesDone() override;

private:
    Executor _executor;
    RT* _res;
};

template <typename Executor>
class GrpcCaller final : public CallInterface {
public:
    using RT = typename Executor::ResponseType;
    using WT = typename Executor::RequestType;

    GrpcCaller() = delete;
    GrpcCaller(Executor&& executor, const WT& req, RT* res)
        : _executor(std::move(executor)), _req(req), _res(res) {}

    bool Call() override;

private:
    Executor _executor;
    const WT& _req;
    RT* _res;
};

template <typename W, typename R, typename Func, typename Stub>
inline RpcVisitUniquePtr<GrpcReaderWriter, W, R, Func, Stub>
MakeReaderWriter(Func&& func, Stub* stub, Controller* cntl) { 
	return MakeStreamableVisit<RpcVisit<GrpcReaderWriter, W, R, Func, Stub>>(
		MakeRpcExecutor<W, R>(std::forward<Func>(func), stub, cntl));
}

template <typename R, typename W, typename Func, typename Stub>
inline RpcVisitUniquePtr<GrpcReader, W, R, Func, Stub>
MakeReader(Func&& func, Stub* stub, Controller* cntl, const W& req) { 
	return MakeStreamableVisit<RpcVisit<GrpcReader, W, R, Func, Stub>>(
		MakeRpcExecutor<W, R>(std::forward<Func>(func), stub, cntl), req);
}

template <typename W, typename R, typename Func, typename Stub>
inline RpcVisitUniquePtr<GrpcWriter, W, R, Func, Stub>
MakeWriter(Func&& func, Stub* stub, Controller* cntl, R* res) { 
	return MakeStreamableVisit<RpcVisit<GrpcWriter, W, R, Func, Stub>>(
		MakeRpcExecutor<W, R>(std::forward<Func>(func), stub, cntl), res);
}

template <typename W, typename R, typename Func, typename Stub>
inline RpcVisitUniquePtr<GrpcCaller, W, R, Func, Stub>
MakeCaller(Func&& func, Stub* stub, Controller* cntl, const W& req, R* res) { 
	return MakeStreamableVisit<RpcVisit<GrpcCaller, W, R, Func, Stub>>(
		MakeRpcExecutor<W, R>(std::forward<Func>(func), stub, cntl), req, res);
}
} // namespace grpcvisit

} // namespace experimental
} // namespace brpc

#include "brpc/grpc_stream_inl.h"

#endif // BRPC_GRPC_STREAM_H