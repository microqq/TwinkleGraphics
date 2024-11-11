#ifndef TW_ASSETS_WINDOW
#define TW_ASSETS_WINDOW

#include <map>
#include <unordered_map>
#include <vector>

#include <filesystem>

#include "imgui.h"
#include "twWindow.h"

namespace TwinkleGraphics {

using namespace std::chrono_literals;

namespace fs = std::filesystem;
static constexpr int TILE_SIZE = 96;

enum AssetStyleFlags {
  AssetStyleFlags_NONE = 0,
  AssetStyleFlags_BY_TYPE_FILE = (1 << 0),
  AssetStyleFlags_BY_TYPE_DIR = (1 << 1),
  AssetStyleFlags_BY_TYPE_LINK = (1 << 2),
  AssetStyleFlags_BY_TYPE_EXTENSION = (1 << 3),
  AssetStyleFlags_BY_TYPE_FULLNAME = (1 << 4)
};

enum AssetType {
  AssetType_NONE = 0,
  AssetType_FOLDER = 1,
  AssetType_FILE = 2,
  AssetType_IMAGE = 3,
  AssetType_MATERIAL = 4,
  AssetType_SHADER = 5,
  AssetType_TXT = 6,
  AssetType_JSON = 7,
  AssetType_SCRIPT = 8,
  AssetType_PROTO = 9,
  AssetType_MODEL = 10
};

struct AssetStyle {
  AssetStyle();
  AssetStyle(const AssetStyle &style);
  AssetStyle(const ImVec4 &col, const std::string &ico = "",
             ImFont *ft = nullptr);

  ImVec4 color{0, 0, 0, 0};
  std::string icon;
  ImFont *font{nullptr};
};

struct AssetInfo {
  AssetInfo(const fs::path &assetPath, AssetType assetType, bool assetSelected);

  std::string hash;
  std::string path;
  std::string baseName;
  std::string extension;
  std::string fileModifyDate;
  std::string fileSizeString;
  double fileSize{0.0f};
  AssetType type{AssetType::AssetType_NONE};
  bool selected{false};
  bool collapse{true};
};

class __TWAPI AssetsWindow : public Window {
public:
  explicit AssetsWindow(const std::string &name, uint32 width, uint32 height,
                        const std::string &assetsDir, Widget *parent = nullptr);
  virtual ~AssetsWindow();

  virtual void Update(float deltaTime = 0.0f) override;
  virtual void Destroy() override;

  void SetAssetStyle(const AssetStyleFlags &flags, const char *criteria,
                     const AssetStyle &style);
  void SetAssetStyle(const AssetStyleFlags &flags, const char *criterial,
                     const ImVec4 &color, const std::string &icon = "",
                     ImFont *font = nullptr);

  std::shared_ptr<AssetStyle> GetAssetStyle(const AssetStyleFlags &flags,
                                            const char *criteria);

protected:
  virtual void OnGuiBegin() override;
  virtual void OnGui() override;
  virtual void OnGuiEnd() override;

private:
  enum SortTag { NONE = 0, FILENAME = 1, EXTENSION = 2, SIZE = 3, DATE = 4 };

  virtual void SetFocusedInternal() override;
  virtual void SetHoveredInternal() override;

  bool UpdateTreeView();
  void UpdatePreview();
  void CreateFolderTreeNode(const fs::path &path);
  void UpdatePreviewAssets();
  void SortPreviewAssets(SortTag tag = SortTag::FILENAME);

private:
  std::map<std::string, AssetInfo> _assets{};
  std::unordered_map<
      AssetStyleFlags,
      std::unordered_map<std::string, std::shared_ptr<AssetStyle>>>
      _assetsStyle;
  std::vector<AssetInfo *> _selectedAssets{};
  std::vector<AssetInfo *> _previewAssets{};
  std::string _assetsDir;
  std::string _treeSelectedAsset;
  char _searchFilter[128]{};
  float _thumbnailSize{1.0f};
  bool _thumbnailsList{true};
};
} // namespace TwinkleGraphics

#endif
