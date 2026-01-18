# Git Setup Guide for Your GitHub Repo

## Step-by-Step Instructions

### 1. Add Your GitHub Repository as Remote

Replace `YOUR_USERNAME` and `YOUR_REPO_NAME` with your actual GitHub username and repo name:

```bash
git remote add origin https://github.com/YOUR_USERNAME/YOUR_REPO_NAME.git
```

Or if you're using SSH:
```bash
git remote add origin git@github.com:YOUR_USERNAME/YOUR_REPO_NAME.git
```

### 2. Check What Files You Have Changed

```bash
git status
```

### 3. Stage Your Changes

**Option A: Stage specific files**
```bash
git add src/byte_stream.cc
git add src/byte_stream.hh
git add CPP_CONCEPTS_NEEDED.md
git add UNDERSTANDING_THE_CODEBASE.md
git add networking/
```

**Option B: Stage all changes**
```bash
git add .
```

### 4. Make Your First Commit

```bash
git commit -m "Initial commit: Add ByteStream implementation and learning resources"
```

**Good commit message examples:**
- `"Add ByteStream implementation"`
- `"Fix circular dependency in ByteStream"`
- `"Add learning resources and playground code"`
- `"Implement Writer::push() method"`

### 5. Push to GitHub

**If this is your first push:**
```bash
git push -u origin check1-startercode
```

**For subsequent pushes:**
```bash
git push
```

### 6. Create a Main Branch (Optional)

If you want to work on `main` branch instead:

```bash
git checkout -b main
git push -u origin main
```

## Useful Git Commands for Tracking Progress

### Make Small, Frequent Commits

```bash
# Stage specific files
git add src/byte_stream.cc

# Commit with descriptive message
git commit -m "Implement Writer::push() method"

# Push to GitHub
git push
```

### Check Your Commit History

```bash
git log --oneline
```

### See What Changed

```bash
git diff                    # See unstaged changes
git diff --staged          # See staged changes
git log -p                 # See changes in commits
```

### Create a .gitignore (Recommended)

Create a `.gitignore` file to exclude build files:

```
build/
*.o
*.exe
*.out
.DS_Store
```

## Workflow Example

```bash
# 1. Make changes to your code
# 2. Check what changed
git status

# 3. Stage changes
git add src/byte_stream.cc

# 4. Commit with descriptive message
git commit -m "Implement Reader::peek() method"

# 5. Push to GitHub
git push

# Repeat for each feature/change!
```

## Troubleshooting

### If you get "remote origin already exists"
```bash
git remote remove origin
git remote add origin https://github.com/YOUR_USERNAME/YOUR_REPO_NAME.git
```

### If push is rejected
```bash
git pull origin check1-startercode --rebase
git push
```

### To see your remote URL
```bash
git remote -v
```

