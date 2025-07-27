#include "Json_Utils.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonWriter.h"
#include "Serialization/JsonReader.h"
#include "JsonObjectConverter.h"
#include "Misc/FileHelper.h"

DEFINE_LOG_CATEGORY(Json_Utils_Log);

// AInventory_Item
//------------------------------------------------------------------------------------------------------------
bool Json_Utils::Write_Input_Data(const FString &file_name, const FInput_Data& input_data)
{
	FString out_string;
	TSharedPtr<FJsonObject> main_json_object = 0;

	main_json_object = FJsonObjectConverter::UStructToJsonObject(input_data);
	if (!main_json_object.IsValid() )
	{
		UE_LOG(Json_Utils_Log, Error, TEXT(">>>>>>[%hs] Json Object was null"), __FUNCTION__);
		return false;
	}

	TSharedRef<TJsonWriter<> > json_writer = TJsonWriterFactory<>::Create(&out_string);

	if (!FJsonSerializer::Serialize(main_json_object.ToSharedRef(), json_writer) )
	{
		UE_LOG(Json_Utils_Log, Error, TEXT(">>>>>>[%hs] Json Object was not serialized"), __FUNCTION__);
		return false;
	}

	if (!json_writer->Close() )
	{
		UE_LOG(Json_Utils_Log, Error, TEXT(">>>>>>[%hs] Json Writer was not closed"), __FUNCTION__);
		return false;
	}

	if (!FFileHelper::SaveStringToFile(out_string, *file_name) )
	{
		UE_LOG(Json_Utils_Log, Error, TEXT(">>>>>>[%hs] String was not saved into file"), __FUNCTION__);
		return false;
	}

	return true;
}
//------------------------------------------------------------------------------------------------------------
bool Json_Utils::Read_Input_Data(const FString &file_name, FInput_Data &input_data)
{
	FString file_content;
	TSharedPtr<FJsonObject> main_json_object = 0;

	if (!FFileHelper::LoadFileToString(file_content, *file_name) )
	{
		UE_LOG(Json_Utils_Log, Error, TEXT(">>>>>>[%hs] File was not load to string"), __FUNCTION__);
		return false;
	}

	TSharedRef<TJsonReader<> > json_reader = TJsonReaderFactory<>::Create(file_content);

	if (!FJsonSerializer::Deserialize(json_reader, main_json_object) )
	{
		UE_LOG(Json_Utils_Log, Error, TEXT(">>>>>>[%hs] Json String was not deserialized"), __FUNCTION__);
		return false;
	}

	if (!main_json_object.IsValid() )
	{
		UE_LOG(Json_Utils_Log, Error, TEXT(">>>>>>[%hs] Json Object was null"), __FUNCTION__);
		return false;
	}

	if (!FJsonObjectConverter::JsonObjectToUStruct(main_json_object.ToSharedRef(), &input_data) )
	{
		UE_LOG(Json_Utils_Log, Error, TEXT(">>>>>>[%hs] Json Object was not converted to USTRUCT"), __FUNCTION__);
		return false;
	}

	return true;
}
//------------------------------------------------------------------------------------------------------------
