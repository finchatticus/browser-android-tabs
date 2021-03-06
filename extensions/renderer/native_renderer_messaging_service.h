// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef EXTENSIONS_RENDERER_NATIVE_RENDERER_MESSAGING_SERVICE_H_
#define EXTENSIONS_RENDERER_NATIVE_RENDERER_MESSAGING_SERVICE_H_

#include <string>

#include "base/macros.h"
#include "extensions/common/extension_id.h"
#include "extensions/renderer/gin_port.h"
#include "extensions/renderer/one_time_message_handler.h"
#include "extensions/renderer/renderer_messaging_service.h"
#include "gin/handle.h"

struct ExtensionMsg_ExternalConnectionInfo;
struct ExtensionMsg_TabConnectionInfo;

namespace extensions {
class NativeExtensionBindingsSystem;
struct Message;
struct MessageTarget;
struct PortId;

// The messaging service to handle dispatching extension messages and connection
// events to different contexts.
// This primarily handles long-lived port-based communications (like
// runtime.connect). A basic flow will create an "opener" port and one ore more
// "receiver" ports in different contexts (and possibly processes). This class
// manages the communication with the browser to forward these messages along.
// From JavaScript, a basic flow would be:
// // page1.js
// var port = chrome.runtime.connect();
// port.onMessage.addListener(function() { <handle message> });
// port.postMessage('hi!');
// <eventually> port.disconnect();
// // page2.js
// chrome.runtime.onConnect.addListener(function(port) {
//   port.onMessage.addListener(function() { <handle message> });
//   port.postMessage('hey!');
// });
// This causes the following steps in the messaging service:
// Connection:
// * connect() triggers OpenChannelToExtension, which notifies the browser of
//   a new connection.
// * The browser dispatches OnConnect messages to different renderers. If a
//   renderer has a listener, it will respond with an OpenMessagePort message.
//   If no renderer has a listener, the browser will close the port.
// Message Posting
// * Calls to postMessage() trigger a PostMessageToPort messge to the browser.
// * The browser sends a DeliverMessage message to listening renderers. These
//   then dispatch the onMessage event to listeners.
// Disconnecting
// * disconnect() calls result in sending a CloseMessagePort message to the
//   browser.
// * The browser then sends a DispatchOnDisconnect message to other renderers,
//   which triggers the onDisconnect() event.
// TODO(devlin): This is a pretty large comment for a class, and it documents
// browser/renderer interaction. I wonder if this would be better in a
// messaging.md document?
class NativeRendererMessagingService : public RendererMessagingService,
                                       public GinPort::Delegate {
 public:
  explicit NativeRendererMessagingService(
      NativeExtensionBindingsSystem* bindings_system);
  ~NativeRendererMessagingService() override;

  // Creates and opens a new message port in the specified context.
  gin::Handle<GinPort> Connect(ScriptContext* script_context,
                               const MessageTarget& target,
                               const std::string& name,
                               bool include_tls_channel_id);

  // Sends a one-time message, as is used by runtime.sendMessage.
  void SendOneTimeMessage(ScriptContext* script_context,
                          const MessageTarget& target,
                          const std::string& channel_name,
                          bool include_tls_channel_id,
                          const Message& message,
                          v8::Local<v8::Function> response_callback);

  // GinPort::Delegate:
  void PostMessageToPort(v8::Local<v8::Context> context,
                         const PortId& port_id,
                         int routing_id,
                         std::unique_ptr<Message> message) override;
  void ClosePort(v8::Local<v8::Context> context,
                 const PortId& port_id,
                 int routing_id) override;

  gin::Handle<GinPort> CreatePortForTesting(ScriptContext* script_context,
                                            const std::string& channel_name,
                                            const PortId& port_id);
  gin::Handle<GinPort> GetPortForTesting(ScriptContext* script_context,
                                         const PortId& port_id);
  bool HasPortForTesting(ScriptContext* script_context, const PortId& port_id);

 private:
  // RendererMessagingService:
  bool ContextHasMessagePort(ScriptContext* script_context,
                             const PortId& port_id) override;
  void DispatchOnConnectToListeners(
      ScriptContext* script_context,
      const PortId& target_port_id,
      const ExtensionId& target_extension_id,
      const std::string& channel_name,
      const ExtensionMsg_TabConnectionInfo* source,
      const ExtensionMsg_ExternalConnectionInfo& info,
      const std::string& tls_channel_id,
      const std::string& event_name) override;
  void DispatchOnMessageToListeners(ScriptContext* script_context,
                                    const Message& message,
                                    const PortId& target_port_id) override;
  void DispatchOnDisconnectToListeners(ScriptContext* script_context,
                                       const PortId& port_id,
                                       const std::string& error) override;

  // Creates a new port in the given context, with the specified |channel_name|
  // and |port_id|. Assumes no such port exists.
  gin::Handle<GinPort> CreatePort(ScriptContext* script_context,
                                  const std::string& channel_name,
                                  const PortId& port_id);

  // Returns the port with the given |port_id| in the given |script_context|;
  // requires that such a port exists.
  gin::Handle<GinPort> GetPort(ScriptContext* script_context,
                               const PortId& port_id);

  // The associated bindings system; guaranteed to outlive this object.
  NativeExtensionBindingsSystem* const bindings_system_;

  OneTimeMessageHandler one_time_message_handler_;

  DISALLOW_COPY_AND_ASSIGN(NativeRendererMessagingService);
};

}  // namespace extensions

#endif  // EXTENSIONS_RENDERER_NATIVE_RENDERER_MESSAGING_SERVICE_H_
