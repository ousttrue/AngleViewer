#pragma once
#include <string>
#include <memory>


namespace Microsoft {
	namespace glTF {
		class Document;
	}
}

std::shared_ptr<Microsoft::glTF::Document> LoadGLTF(const std::wstring &path);
