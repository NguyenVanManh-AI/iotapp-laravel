<?php

namespace App\Http\Controllers;

use Illuminate\Http\Request;
use Illuminate\Support\Facades\Http;
use Illuminate\Support\Facades\Log;
use Throwable;

class IOTController extends Controller
{

    public function uploadaudio(Request $request) 
    {
        if (!$request->hasFile('audio')) {
            return response()->json([
                'status' => 'fail',
                'message' => 'Error. No audio files sent !',
            ]);
        }
    
        $audioFile = $request->file('audio');
        $fileName = time() . '.' . $audioFile->getClientOriginalExtension();
        $audioFile->storeAs('audio', $fileName, 'public'); // lưu file không tốn thời gian bằng gửi file lên ngrock 
    
        try {
            // Sử dụng stream thay vì file_get_contents
            $fileStream = fopen($audioFile->getRealPath(), 'r');

            // Cấu hình thời gian chờ thấp hơn, ví dụ: 120 giây
            $response = Http::timeout(120)
                ->attach('file', $fileStream, $fileName)
                ->post('https://c93d-34-82-65-32.ngrok-free.app/transcribe');

            fclose($fileStream); // Đóng stream sau khi sử dụng

            // Check if the transcription API response was successful
            if (!$response->successful()) {
                return response()->json([
                    'status' => 'fail',
                    'message' => 'Error when calling model !',
                ]);
            }
    
            $transcriptionData = $response->json();
            Log::info($transcriptionData['index']); 
            Log::info($transcriptionData['transcription']); 
            
            $option = $transcriptionData['index'];

            // sleep(5);
            // $numbers = [0, 5, 6, 7, 8, 9, 10, 0];
            // $option = $numbers[array_rand($numbers)];
            // $transcriptionData['transcription'] = 'ABC successful !' . $option;

            // Check for invalid option
            if ($option == 0) {
                return response()->json([
                    'status' => 'fail',
                    'message' => 'Invalid action !',
                ]);
            }
    
            // Send option to ESP8266
            $url = "http://192.168.68.168/sendOption";
            $data = ['option' => $option];
            $espResponse = Http::asForm()->post($url, $data);
    
            // Check if ESP8266 response was successful
            if (!$espResponse->successful()) {
                return response()->json([
                    'status' => 'fail',
                    'message' => 'Error when calling ESP8266 !',
                ]);
            }
    
            return response()->json([
                'status' => 'ok',
                'message' => 'Successful control !',
                'transcription' => $transcriptionData['transcription'] ?? 'Successful control',
                'option' => $option,
            ]);
    
        } catch (Throwable $e) {
            Log::error($e->getMessage());  
            return response()->json([
                'status' => 'fail',
                'message' => 'Error when processing audio files !',
            ]);
        }
    }
    
}
