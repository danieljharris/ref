# Publish to GitHub

This folder is a standalone git repo (branch `main`), ready to push.

```bash
cd /workspaces/ref
gh auth login
gh repo create ref --public --description "C++23 header-only Ref<T>: non-null shared handle" --source=. --remote=origin --push
```

Then in other projects, add as a submodule:

```bash
git submodule add https://github.com/danieljharris/ref.git third_party/ref
```

Include: `third_party/ref/include` and `#include "ref.hpp"`.