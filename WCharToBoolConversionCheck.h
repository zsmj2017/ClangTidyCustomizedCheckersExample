#ifndef WCHAR_TO_BOOL_CONVERSION_CHECK_H
#define WCHAR_TO_BOOL_CONVERSION_CHECK_H

#include "clang-tidy/ClangTidyCheck.h"

namespace clang {
namespace tidy {

class WCharToBoolConversionCheck : public ClangTidyCheck {
public:
  WCharToBoolConversionCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace tidy
} // namespace clang

#endif
