// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_BLINK_PUBLIC_WEB_MODULES_MEDIASTREAM_MOCK_MEDIA_STREAM_VIDEO_SINK_H_
#define THIRD_PARTY_BLINK_PUBLIC_WEB_MODULES_MEDIASTREAM_MOCK_MEDIA_STREAM_VIDEO_SINK_H_

#include "third_party/blink/public/web/modules/mediastream/media_stream_video_sink.h"

#include "base/memory/weak_ptr.h"
#include "media/base/video_frame.h"
#include "testing/gmock/include/gmock/gmock.h"
#include "third_party/blink/public/common/media/video_capture.h"

namespace blink {

class MockMediaStreamVideoSink : public MediaStreamVideoSink {
 public:
  MockMediaStreamVideoSink();
  ~MockMediaStreamVideoSink() override;

  void ConnectToTrack(const WebMediaStreamTrack& track) {
    MediaStreamVideoSink::ConnectToTrack(track, GetDeliverFrameCB(), true);
  }

  void ConnectToTrackWithCallback(const WebMediaStreamTrack& track,
                                  const VideoCaptureDeliverFrameCB& callback) {
    MediaStreamVideoSink::ConnectToTrack(track, callback, true);
  }

  void DisconnectFromTrack() { MediaStreamVideoSink::DisconnectFromTrack(); }

  void OnReadyStateChanged(WebMediaStreamSource::ReadyState state) override;
  void OnEnabledChanged(bool enabled) override;

  // Triggered when OnVideoFrame(scoped_refptr<media::VideoFrame> frame)
  // is called.
  MOCK_METHOD0(OnVideoFrame, void());

  VideoCaptureDeliverFrameCB GetDeliverFrameCB();

  int number_of_frames() const { return number_of_frames_; }
  media::VideoPixelFormat format() const { return format_; }
  gfx::Size frame_size() const { return frame_size_; }
  scoped_refptr<media::VideoFrame> last_frame() const { return last_frame_; }

  bool enabled() const { return enabled_; }
  WebMediaStreamSource::ReadyState state() const { return state_; }

 private:
  void DeliverVideoFrame(scoped_refptr<media::VideoFrame> frame,
                         base::TimeTicks estimated_capture_time);

  int number_of_frames_;
  bool enabled_;
  media::VideoPixelFormat format_;
  WebMediaStreamSource::ReadyState state_;
  gfx::Size frame_size_;
  scoped_refptr<media::VideoFrame> last_frame_;
  base::WeakPtrFactory<MockMediaStreamVideoSink> weak_factory_;
};

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_PUBLIC_WEB_MODULES_MEDIASTREAM_MOCK_MEDIA_STREAM_VIDEO_SINK_H_