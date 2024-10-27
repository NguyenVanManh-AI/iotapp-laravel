<?php

use Illuminate\Http\Request;
use Illuminate\Support\Facades\Route;

/*
|--------------------------------------------------------------------------
| API Routes
|--------------------------------------------------------------------------
|
| Here is where you can register API routes for your application. These
| routes are loaded by the RouteServiceProvider within a group which
| is assigned the "api" middleware group. Enjoy building your API!
|
*/

Route::post('/upload-audio', [\App\Http\Controllers\IOTController::class, 'uploadaudio']);
Route::post('/upload-text', [\App\Http\Controllers\IOTController::class, 'uploadText']);
Route::post('/upload-image', [\App\Http\Controllers\IOTController::class, 'uploadImage']);

Route::middleware('auth:sanctum')->get('/user', function (Request $request) {
    return $request->user();
});
