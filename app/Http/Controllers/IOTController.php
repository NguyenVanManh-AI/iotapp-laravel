<?php

namespace App\Http\Controllers;

use Illuminate\Http\Request;
use Illuminate\Support\Facades\Http;


class IOTController extends Controller
{
    public function uploadaudio(Request $request) 
    {
        $request->validate([
            'audio' => 'required',
        ]);
    
        if ($request->hasFile('audio')) {
            $audioFile = $request->file('audio');
            $fileName = time() . '.' . $audioFile->getClientOriginalExtension();
            $audioFile->storeAs('audio', $fileName, 'public');
    
            $options = [1, 3, 5, 7, 9];
            // $options = [1];
            $option = $options[array_rand($options)];
    
            $url = "http://192.168.89.168/sendOption";
            $data = ['option' => $option];
    
            try {
                // Gửi yêu cầu với dữ liệu dưới dạng form
                $response = Http::asForm()->post($url, $data);
    
                if ($response->successful()) {
                    return response()->json([
                        'success' => true,
                        'message' => 'Tệp âm thanh đã được tải lên thành công.',
                        'file_name' => $fileName,
                        'file_path' => asset('storage/audio/' . $fileName),
                        'api_response' => $response->json(),
                    ]);
                } else {
                    return response()->json([
                        'success' => false,
                        'message' => 'Không thể gửi yêu cầu đến ESP8266.',
                        'status_code' => $response->status(),
                        'error' => $response->body(),
                    ]);
                }
            } catch (\Exception $e) {
                return response()->json([
                    'success' => false,
                    'message' => 'Lỗi khi gửi yêu cầu đến ESP8266.',
                    'error' => $e->getMessage(),
                ]);
            }
        }
    
        return response()->json([
            'success' => false,
            'message' => 'Không có tệp âm thanh nào được gửi.',
        ]);
    }

    public function uploadText(Request $request)
    {
        // Xác thực văn bản
        $request->validate([
            'text' => 'required|string|max:255', // Giới hạn độ dài văn bản tối đa 255 ký tự
        ]);

        // Lấy văn bản từ yêu cầu
        $text = $request->input('text');

        // Xử lý văn bản nếu cần (ví dụ: lưu vào cơ sở dữ liệu)
        // Ở đây, mình sẽ chỉ trả về phản hồi thành công
        return response()->json([
            'success' => true,
            'message' => 'Văn bản đã được tải lên thành công.',
            'text' => $text, // Trả lại văn bản đã gửi
        ]);
    }

    public function uploadImage(Request $request)
    {
        // Xác thực tệp hình ảnh
        $request->validate([
            'image' => 'required|image|mimes:jpeg,png,jpg,gif|max:20480', // Giới hạn kích thước tệp tối đa 20MB
        ]);

        // Lưu tệp hình ảnh
        if ($request->hasFile('image')) {
            $imageFile = $request->file('image');
            $fileName = time() . '.' . $imageFile->getClientOriginalExtension(); // Tạo tên tệp duy nhất
            $imageFile->storeAs('images', $fileName, 'public'); // Lưu tệp vào thư mục 'storage/app/public/images'

            // Trả về phản hồi
            return response()->json([
                'success' => true,
                'message' => 'Tệp hình ảnh đã được tải lên thành công.',
                'file_name' => $fileName,
                'file_path' => asset('storage/images/' . $fileName), // Đường dẫn đến tệp
            ]);
        }

        // Trả về phản hồi nếu không có tệp
        return response()->json([
            'success' => false,
            'message' => 'Không có tệp hình ảnh nào được gửi.',
        ]);
    }
}
