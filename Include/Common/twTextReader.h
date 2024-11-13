#ifndef TW_TEXTREADER_H
#define TW_TEXTREADER_H

#include "twResource.h"
#include "twSingleton.h"

namespace TwinkleGraphics {
struct TextSource : public Object {
  using Ptr = std::shared_ptr<TextSource>;
  using WeakPtr = std::weak_ptr<TextSource>;

  std::string filename;
  std::string content;
};

class TextReader : public ResourceReader,
                   public Reference<TextReader>,
                   public INonCopyable {
public:
  using Ptr = std::shared_ptr<TextReader>;

  TextReader();
  TextReader(ReaderOption *option);
  virtual ~TextReader();

  ReadResult<TextSource> Read(const char *filename);
  ReadResult<TextSource> ReadAsync(std::string filename);

  void SetOption(ReaderOption *option) {
    if (option == nullptr)
      return;

    if (_option != nullptr) {
      SAFE_DEL(_option);
    }
    _option = new ReaderOption(*option);
  }
  DECLARE_READERID;
};

} // namespace TwinkleGraphics
#endif
