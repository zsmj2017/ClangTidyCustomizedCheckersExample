#include "clang-tidy/ClangTidy.h"
#include "clang-tidy/ClangTidyCheck.h"
#include "clang-tidy/ClangTidyModule.h"
#include "clang-tidy/ClangTidyModuleRegistry.h"

namespace clang {
namespace tidy {

class WCharToBoolConversionCheck : public ClangTidyCheck {
public:
  WCharToBoolConversionCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  void registerMatchers(ast_matchers::MatchFinder *Finder) {
    using namespace clang::ast_matchers;

    // Match function calls and constructor calls with a boolean parameter.
    Finder->addMatcher(
        callExpr(forEachArgumentWithParam(expr().bind("arg"),
                                          parmVarDecl(hasType(booleanType()))))
            .bind("call"),
        this);

    Finder->addMatcher(
        cxxConstructExpr(
            forEachArgumentWithParam(expr().bind("arg"),
                                     parmVarDecl(hasType(booleanType()))))
            .bind("construct"),
        this);
  }

  void check(const ast_matchers::MatchFinder::MatchResult &Result) {
    const auto *ArgExpr = Result.Nodes.getNodeAs<Expr>("arg");

    if (!ArgExpr) {
      return;
    }

    QualType ArgType = ArgExpr->getType().getCanonicalType();

    if (const auto *PT = dyn_cast<PointerType>(ArgType)) {
      QualType PointeeType = PT->getPointeeType().getCanonicalType();
      // If the pointee type is wchar_t || const wchar_t, emit a warning.
      if (PointeeType->isWideCharType() ||
          (PointeeType.isConstQualified() &&
           PointeeType.getUnqualifiedType()->isWideCharType())) {
        diag(ArgExpr->getBeginLoc(),
             "passing 'wchar_t*' to a boolean parameter, which may lead to "
             "unexpected behavior");
      }
    }
  }
};

class WCharToBoolConversionModule : public ClangTidyModule {
public:
  void addCheckFactories(ClangTidyCheckFactories &CheckFactories) override {
    CheckFactories.registerCheck<WCharToBoolConversionCheck>(
        "wchar-to-bool-conversion-check");
  }
};

extern "C" ClangTidyModuleRegistry::Add<WCharToBoolConversionModule>
    X("wchar-to-bool-conversion-module",
      "Adds checks for wchar_t* to bool conversions.");

} // namespace tidy
} // namespace clang
