// WARNING: This file is machine generated by fidlgen.

#include <fuchsia/hardware/spi/llcpp/fidl.h>
#include <memory>

namespace llcpp {

namespace fuchsia {
namespace hardware {
namespace spi {

namespace {

[[maybe_unused]]
constexpr uint64_t kDevice_Transmit_Ordinal = 0x589d02df00000000lu;
extern "C" const fidl_type_t fuchsia_hardware_spi_DeviceTransmitRequestTable;
extern "C" const fidl_type_t fuchsia_hardware_spi_DeviceTransmitResponseTable;
[[maybe_unused]]
constexpr uint64_t kDevice_Receive_Ordinal = 0x58529d900000000lu;
extern "C" const fidl_type_t fuchsia_hardware_spi_DeviceReceiveRequestTable;
extern "C" const fidl_type_t fuchsia_hardware_spi_DeviceReceiveResponseTable;
[[maybe_unused]]
constexpr uint64_t kDevice_Exchange_Ordinal = 0x13c3394e00000000lu;
extern "C" const fidl_type_t fuchsia_hardware_spi_DeviceExchangeRequestTable;
extern "C" const fidl_type_t fuchsia_hardware_spi_DeviceExchangeResponseTable;

}  // namespace
template <>
Device::ResultOf::Transmit_Impl<Device::TransmitResponse>::Transmit_Impl(zx::unowned_channel _client_end, ::fidl::VectorView<uint8_t> data) {
  constexpr uint32_t _kWriteAllocSize = ::fidl::internal::ClampedMessageSize<TransmitRequest, ::fidl::MessageDirection::kSending>();
  std::unique_ptr _write_bytes_boxed = std::make_unique<::fidl::internal::AlignedBuffer<_kWriteAllocSize>>();
  auto& _write_bytes_array = *_write_bytes_boxed;
  TransmitRequest _request = {};
  _request.data = std::move(data);
  auto _linearize_result = ::fidl::Linearize(&_request, _write_bytes_array.view());
  if (_linearize_result.status != ZX_OK) {
    Super::SetFailure(std::move(_linearize_result));
    return;
  }
  ::fidl::DecodedMessage<TransmitRequest> _decoded_request = std::move(_linearize_result.message);
  Super::SetResult(
      Device::InPlace::Transmit(std::move(_client_end), std::move(_decoded_request), Super::response_buffer()));
}

Device::ResultOf::Transmit Device::SyncClient::Transmit(::fidl::VectorView<uint8_t> data) {
  return ResultOf::Transmit(zx::unowned_channel(this->channel_), std::move(data));
}

Device::ResultOf::Transmit Device::Call::Transmit(zx::unowned_channel _client_end, ::fidl::VectorView<uint8_t> data) {
  return ResultOf::Transmit(std::move(_client_end), std::move(data));
}

template <>
Device::UnownedResultOf::Transmit_Impl<Device::TransmitResponse>::Transmit_Impl(zx::unowned_channel _client_end, ::fidl::BytePart _request_buffer, ::fidl::VectorView<uint8_t> data, ::fidl::BytePart _response_buffer) {
  if (_request_buffer.capacity() < TransmitRequest::PrimarySize) {
    Super::SetFailure(::fidl::DecodeResult<TransmitResponse>(ZX_ERR_BUFFER_TOO_SMALL, ::fidl::internal::kErrorRequestBufferTooSmall));
    return;
  }
  TransmitRequest _request = {};
  _request.data = std::move(data);
  auto _linearize_result = ::fidl::Linearize(&_request, std::move(_request_buffer));
  if (_linearize_result.status != ZX_OK) {
    Super::SetFailure(std::move(_linearize_result));
    return;
  }
  ::fidl::DecodedMessage<TransmitRequest> _decoded_request = std::move(_linearize_result.message);
  Super::SetResult(
      Device::InPlace::Transmit(std::move(_client_end), std::move(_decoded_request), std::move(_response_buffer)));
}

Device::UnownedResultOf::Transmit Device::SyncClient::Transmit(::fidl::BytePart _request_buffer, ::fidl::VectorView<uint8_t> data, ::fidl::BytePart _response_buffer) {
  return UnownedResultOf::Transmit(zx::unowned_channel(this->channel_), std::move(_request_buffer), std::move(data), std::move(_response_buffer));
}

Device::UnownedResultOf::Transmit Device::Call::Transmit(zx::unowned_channel _client_end, ::fidl::BytePart _request_buffer, ::fidl::VectorView<uint8_t> data, ::fidl::BytePart _response_buffer) {
  return UnownedResultOf::Transmit(std::move(_client_end), std::move(_request_buffer), std::move(data), std::move(_response_buffer));
}

::fidl::DecodeResult<Device::TransmitResponse> Device::InPlace::Transmit(zx::unowned_channel _client_end, ::fidl::DecodedMessage<TransmitRequest> params, ::fidl::BytePart response_buffer) {
  params.message()->_hdr = {};
  params.message()->_hdr.ordinal = kDevice_Transmit_Ordinal;
  auto _encode_request_result = ::fidl::Encode(std::move(params));
  if (_encode_request_result.status != ZX_OK) {
    return ::fidl::DecodeResult<Device::TransmitResponse>::FromFailure(
        std::move(_encode_request_result));
  }
  auto _call_result = ::fidl::Call<TransmitRequest, TransmitResponse>(
    std::move(_client_end), std::move(_encode_request_result.message), std::move(response_buffer));
  if (_call_result.status != ZX_OK) {
    return ::fidl::DecodeResult<Device::TransmitResponse>::FromFailure(
        std::move(_call_result));
  }
  return ::fidl::Decode(std::move(_call_result.message));
}

template <>
Device::ResultOf::Receive_Impl<Device::ReceiveResponse>::Receive_Impl(zx::unowned_channel _client_end, uint32_t size) {
  constexpr uint32_t _kWriteAllocSize = ::fidl::internal::ClampedMessageSize<ReceiveRequest, ::fidl::MessageDirection::kSending>();
  ::fidl::internal::AlignedBuffer<_kWriteAllocSize> _write_bytes_inlined;
  auto& _write_bytes_array = _write_bytes_inlined;
  uint8_t* _write_bytes = _write_bytes_array.view().data();
  memset(_write_bytes, 0, ReceiveRequest::PrimarySize);
  auto& _request = *reinterpret_cast<ReceiveRequest*>(_write_bytes);
  _request.size = std::move(size);
  ::fidl::BytePart _request_bytes(_write_bytes, _kWriteAllocSize, sizeof(ReceiveRequest));
  ::fidl::DecodedMessage<ReceiveRequest> _decoded_request(std::move(_request_bytes));
  Super::SetResult(
      Device::InPlace::Receive(std::move(_client_end), std::move(_decoded_request), Super::response_buffer()));
}

Device::ResultOf::Receive Device::SyncClient::Receive(uint32_t size) {
  return ResultOf::Receive(zx::unowned_channel(this->channel_), std::move(size));
}

Device::ResultOf::Receive Device::Call::Receive(zx::unowned_channel _client_end, uint32_t size) {
  return ResultOf::Receive(std::move(_client_end), std::move(size));
}

template <>
Device::UnownedResultOf::Receive_Impl<Device::ReceiveResponse>::Receive_Impl(zx::unowned_channel _client_end, ::fidl::BytePart _request_buffer, uint32_t size, ::fidl::BytePart _response_buffer) {
  if (_request_buffer.capacity() < ReceiveRequest::PrimarySize) {
    Super::SetFailure(::fidl::DecodeResult<ReceiveResponse>(ZX_ERR_BUFFER_TOO_SMALL, ::fidl::internal::kErrorRequestBufferTooSmall));
    return;
  }
  memset(_request_buffer.data(), 0, ReceiveRequest::PrimarySize);
  auto& _request = *reinterpret_cast<ReceiveRequest*>(_request_buffer.data());
  _request.size = std::move(size);
  _request_buffer.set_actual(sizeof(ReceiveRequest));
  ::fidl::DecodedMessage<ReceiveRequest> _decoded_request(std::move(_request_buffer));
  Super::SetResult(
      Device::InPlace::Receive(std::move(_client_end), std::move(_decoded_request), std::move(_response_buffer)));
}

Device::UnownedResultOf::Receive Device::SyncClient::Receive(::fidl::BytePart _request_buffer, uint32_t size, ::fidl::BytePart _response_buffer) {
  return UnownedResultOf::Receive(zx::unowned_channel(this->channel_), std::move(_request_buffer), std::move(size), std::move(_response_buffer));
}

Device::UnownedResultOf::Receive Device::Call::Receive(zx::unowned_channel _client_end, ::fidl::BytePart _request_buffer, uint32_t size, ::fidl::BytePart _response_buffer) {
  return UnownedResultOf::Receive(std::move(_client_end), std::move(_request_buffer), std::move(size), std::move(_response_buffer));
}

::fidl::DecodeResult<Device::ReceiveResponse> Device::InPlace::Receive(zx::unowned_channel _client_end, ::fidl::DecodedMessage<ReceiveRequest> params, ::fidl::BytePart response_buffer) {
  params.message()->_hdr = {};
  params.message()->_hdr.ordinal = kDevice_Receive_Ordinal;
  auto _encode_request_result = ::fidl::Encode(std::move(params));
  if (_encode_request_result.status != ZX_OK) {
    return ::fidl::DecodeResult<Device::ReceiveResponse>::FromFailure(
        std::move(_encode_request_result));
  }
  auto _call_result = ::fidl::Call<ReceiveRequest, ReceiveResponse>(
    std::move(_client_end), std::move(_encode_request_result.message), std::move(response_buffer));
  if (_call_result.status != ZX_OK) {
    return ::fidl::DecodeResult<Device::ReceiveResponse>::FromFailure(
        std::move(_call_result));
  }
  return ::fidl::Decode(std::move(_call_result.message));
}

template <>
Device::ResultOf::Exchange_Impl<Device::ExchangeResponse>::Exchange_Impl(zx::unowned_channel _client_end, ::fidl::VectorView<uint8_t> txdata) {
  constexpr uint32_t _kWriteAllocSize = ::fidl::internal::ClampedMessageSize<ExchangeRequest, ::fidl::MessageDirection::kSending>();
  std::unique_ptr _write_bytes_boxed = std::make_unique<::fidl::internal::AlignedBuffer<_kWriteAllocSize>>();
  auto& _write_bytes_array = *_write_bytes_boxed;
  ExchangeRequest _request = {};
  _request.txdata = std::move(txdata);
  auto _linearize_result = ::fidl::Linearize(&_request, _write_bytes_array.view());
  if (_linearize_result.status != ZX_OK) {
    Super::SetFailure(std::move(_linearize_result));
    return;
  }
  ::fidl::DecodedMessage<ExchangeRequest> _decoded_request = std::move(_linearize_result.message);
  Super::SetResult(
      Device::InPlace::Exchange(std::move(_client_end), std::move(_decoded_request), Super::response_buffer()));
}

Device::ResultOf::Exchange Device::SyncClient::Exchange(::fidl::VectorView<uint8_t> txdata) {
  return ResultOf::Exchange(zx::unowned_channel(this->channel_), std::move(txdata));
}

Device::ResultOf::Exchange Device::Call::Exchange(zx::unowned_channel _client_end, ::fidl::VectorView<uint8_t> txdata) {
  return ResultOf::Exchange(std::move(_client_end), std::move(txdata));
}

template <>
Device::UnownedResultOf::Exchange_Impl<Device::ExchangeResponse>::Exchange_Impl(zx::unowned_channel _client_end, ::fidl::BytePart _request_buffer, ::fidl::VectorView<uint8_t> txdata, ::fidl::BytePart _response_buffer) {
  if (_request_buffer.capacity() < ExchangeRequest::PrimarySize) {
    Super::SetFailure(::fidl::DecodeResult<ExchangeResponse>(ZX_ERR_BUFFER_TOO_SMALL, ::fidl::internal::kErrorRequestBufferTooSmall));
    return;
  }
  ExchangeRequest _request = {};
  _request.txdata = std::move(txdata);
  auto _linearize_result = ::fidl::Linearize(&_request, std::move(_request_buffer));
  if (_linearize_result.status != ZX_OK) {
    Super::SetFailure(std::move(_linearize_result));
    return;
  }
  ::fidl::DecodedMessage<ExchangeRequest> _decoded_request = std::move(_linearize_result.message);
  Super::SetResult(
      Device::InPlace::Exchange(std::move(_client_end), std::move(_decoded_request), std::move(_response_buffer)));
}

Device::UnownedResultOf::Exchange Device::SyncClient::Exchange(::fidl::BytePart _request_buffer, ::fidl::VectorView<uint8_t> txdata, ::fidl::BytePart _response_buffer) {
  return UnownedResultOf::Exchange(zx::unowned_channel(this->channel_), std::move(_request_buffer), std::move(txdata), std::move(_response_buffer));
}

Device::UnownedResultOf::Exchange Device::Call::Exchange(zx::unowned_channel _client_end, ::fidl::BytePart _request_buffer, ::fidl::VectorView<uint8_t> txdata, ::fidl::BytePart _response_buffer) {
  return UnownedResultOf::Exchange(std::move(_client_end), std::move(_request_buffer), std::move(txdata), std::move(_response_buffer));
}

::fidl::DecodeResult<Device::ExchangeResponse> Device::InPlace::Exchange(zx::unowned_channel _client_end, ::fidl::DecodedMessage<ExchangeRequest> params, ::fidl::BytePart response_buffer) {
  params.message()->_hdr = {};
  params.message()->_hdr.ordinal = kDevice_Exchange_Ordinal;
  auto _encode_request_result = ::fidl::Encode(std::move(params));
  if (_encode_request_result.status != ZX_OK) {
    return ::fidl::DecodeResult<Device::ExchangeResponse>::FromFailure(
        std::move(_encode_request_result));
  }
  auto _call_result = ::fidl::Call<ExchangeRequest, ExchangeResponse>(
    std::move(_client_end), std::move(_encode_request_result.message), std::move(response_buffer));
  if (_call_result.status != ZX_OK) {
    return ::fidl::DecodeResult<Device::ExchangeResponse>::FromFailure(
        std::move(_call_result));
  }
  return ::fidl::Decode(std::move(_call_result.message));
}


bool Device::TryDispatch(Interface* impl, fidl_msg_t* msg, ::fidl::Transaction* txn) {
  if (msg->num_bytes < sizeof(fidl_message_header_t)) {
    zx_handle_close_many(msg->handles, msg->num_handles);
    txn->Close(ZX_ERR_INVALID_ARGS);
    return true;
  }
  fidl_message_header_t* hdr = reinterpret_cast<fidl_message_header_t*>(msg->bytes);
  switch (hdr->ordinal) {
    case kDevice_Transmit_Ordinal:
    {
      auto result = ::fidl::DecodeAs<TransmitRequest>(msg);
      if (result.status != ZX_OK) {
        txn->Close(ZX_ERR_INVALID_ARGS);
        return true;
      }
      auto message = result.message.message();
      impl->Transmit(std::move(message->data),
        Interface::TransmitCompleter::Sync(txn));
      return true;
    }
    case kDevice_Receive_Ordinal:
    {
      auto result = ::fidl::DecodeAs<ReceiveRequest>(msg);
      if (result.status != ZX_OK) {
        txn->Close(ZX_ERR_INVALID_ARGS);
        return true;
      }
      auto message = result.message.message();
      impl->Receive(std::move(message->size),
        Interface::ReceiveCompleter::Sync(txn));
      return true;
    }
    case kDevice_Exchange_Ordinal:
    {
      auto result = ::fidl::DecodeAs<ExchangeRequest>(msg);
      if (result.status != ZX_OK) {
        txn->Close(ZX_ERR_INVALID_ARGS);
        return true;
      }
      auto message = result.message.message();
      impl->Exchange(std::move(message->txdata),
        Interface::ExchangeCompleter::Sync(txn));
      return true;
    }
    default: {
      return false;
    }
  }
}

bool Device::Dispatch(Interface* impl, fidl_msg_t* msg, ::fidl::Transaction* txn) {
  bool found = TryDispatch(impl, msg, txn);
  if (!found) {
    zx_handle_close_many(msg->handles, msg->num_handles);
    txn->Close(ZX_ERR_NOT_SUPPORTED);
  }
  return found;
}


void Device::Interface::TransmitCompleterBase::Reply(int32_t status) {
  constexpr uint32_t _kWriteAllocSize = ::fidl::internal::ClampedMessageSize<TransmitResponse, ::fidl::MessageDirection::kSending>();
  FIDL_ALIGNDECL uint8_t _write_bytes[_kWriteAllocSize] = {};
  auto& _response = *reinterpret_cast<TransmitResponse*>(_write_bytes);
  _response._hdr.ordinal = kDevice_Transmit_Ordinal;
  _response.status = std::move(status);
  ::fidl::BytePart _response_bytes(_write_bytes, _kWriteAllocSize, sizeof(TransmitResponse));
  CompleterBase::SendReply(::fidl::DecodedMessage<TransmitResponse>(std::move(_response_bytes)));
}

void Device::Interface::TransmitCompleterBase::Reply(::fidl::BytePart _buffer, int32_t status) {
  if (_buffer.capacity() < TransmitResponse::PrimarySize) {
    CompleterBase::Close(ZX_ERR_INTERNAL);
    return;
  }
  auto& _response = *reinterpret_cast<TransmitResponse*>(_buffer.data());
  _response._hdr.ordinal = kDevice_Transmit_Ordinal;
  _response.status = std::move(status);
  _buffer.set_actual(sizeof(TransmitResponse));
  CompleterBase::SendReply(::fidl::DecodedMessage<TransmitResponse>(std::move(_buffer)));
}

void Device::Interface::TransmitCompleterBase::Reply(::fidl::DecodedMessage<TransmitResponse> params) {
  params.message()->_hdr = {};
  params.message()->_hdr.ordinal = kDevice_Transmit_Ordinal;
  CompleterBase::SendReply(std::move(params));
}


void Device::Interface::ReceiveCompleterBase::Reply(int32_t status, ::fidl::VectorView<uint8_t> data) {
  constexpr uint32_t _kWriteAllocSize = ::fidl::internal::ClampedMessageSize<ReceiveResponse, ::fidl::MessageDirection::kSending>();
  std::unique_ptr<uint8_t[]> _write_bytes_unique_ptr(new uint8_t[_kWriteAllocSize]);
  uint8_t* _write_bytes = _write_bytes_unique_ptr.get();
  ReceiveResponse _response = {};
  _response._hdr.ordinal = kDevice_Receive_Ordinal;
  _response.status = std::move(status);
  _response.data = std::move(data);
  auto _linearize_result = ::fidl::Linearize(&_response, ::fidl::BytePart(_write_bytes,
                                                                          _kWriteAllocSize));
  if (_linearize_result.status != ZX_OK) {
    CompleterBase::Close(ZX_ERR_INTERNAL);
    return;
  }
  CompleterBase::SendReply(std::move(_linearize_result.message));
}

void Device::Interface::ReceiveCompleterBase::Reply(::fidl::BytePart _buffer, int32_t status, ::fidl::VectorView<uint8_t> data) {
  if (_buffer.capacity() < ReceiveResponse::PrimarySize) {
    CompleterBase::Close(ZX_ERR_INTERNAL);
    return;
  }
  ReceiveResponse _response = {};
  _response._hdr.ordinal = kDevice_Receive_Ordinal;
  _response.status = std::move(status);
  _response.data = std::move(data);
  auto _linearize_result = ::fidl::Linearize(&_response, std::move(_buffer));
  if (_linearize_result.status != ZX_OK) {
    CompleterBase::Close(ZX_ERR_INTERNAL);
    return;
  }
  CompleterBase::SendReply(std::move(_linearize_result.message));
}

void Device::Interface::ReceiveCompleterBase::Reply(::fidl::DecodedMessage<ReceiveResponse> params) {
  params.message()->_hdr = {};
  params.message()->_hdr.ordinal = kDevice_Receive_Ordinal;
  CompleterBase::SendReply(std::move(params));
}


void Device::Interface::ExchangeCompleterBase::Reply(int32_t status, ::fidl::VectorView<uint8_t> rxdata) {
  constexpr uint32_t _kWriteAllocSize = ::fidl::internal::ClampedMessageSize<ExchangeResponse, ::fidl::MessageDirection::kSending>();
  std::unique_ptr<uint8_t[]> _write_bytes_unique_ptr(new uint8_t[_kWriteAllocSize]);
  uint8_t* _write_bytes = _write_bytes_unique_ptr.get();
  ExchangeResponse _response = {};
  _response._hdr.ordinal = kDevice_Exchange_Ordinal;
  _response.status = std::move(status);
  _response.rxdata = std::move(rxdata);
  auto _linearize_result = ::fidl::Linearize(&_response, ::fidl::BytePart(_write_bytes,
                                                                          _kWriteAllocSize));
  if (_linearize_result.status != ZX_OK) {
    CompleterBase::Close(ZX_ERR_INTERNAL);
    return;
  }
  CompleterBase::SendReply(std::move(_linearize_result.message));
}

void Device::Interface::ExchangeCompleterBase::Reply(::fidl::BytePart _buffer, int32_t status, ::fidl::VectorView<uint8_t> rxdata) {
  if (_buffer.capacity() < ExchangeResponse::PrimarySize) {
    CompleterBase::Close(ZX_ERR_INTERNAL);
    return;
  }
  ExchangeResponse _response = {};
  _response._hdr.ordinal = kDevice_Exchange_Ordinal;
  _response.status = std::move(status);
  _response.rxdata = std::move(rxdata);
  auto _linearize_result = ::fidl::Linearize(&_response, std::move(_buffer));
  if (_linearize_result.status != ZX_OK) {
    CompleterBase::Close(ZX_ERR_INTERNAL);
    return;
  }
  CompleterBase::SendReply(std::move(_linearize_result.message));
}

void Device::Interface::ExchangeCompleterBase::Reply(::fidl::DecodedMessage<ExchangeResponse> params) {
  params.message()->_hdr = {};
  params.message()->_hdr.ordinal = kDevice_Exchange_Ordinal;
  CompleterBase::SendReply(std::move(params));
}


}  // namespace spi
}  // namespace hardware
}  // namespace fuchsia
}  // namespace llcpp