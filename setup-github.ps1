# GitHub Repository Setup Helper
# Run this after creating the repository on GitHub

Write-Host "GitHub Repository Setup Helper" -ForegroundColor Cyan
Write-Host ("="*80)

# Check if we're in the right directory
if (!(Test-Path "CMakeLists.txt")) {
    Write-Host "Error: Run this from test-cpp-enterprise directory" -ForegroundColor Red
    Write-Host "   cd C:\BuildTriageSystem\test-cpp-enterprise" -ForegroundColor Yellow
    exit 1
}

# Get GitHub username
Write-Host "`nEnter your GitHub username:" -ForegroundColor Yellow
$username = Read-Host "Username"

if ([string]::IsNullOrWhiteSpace($username)) {
    Write-Host "Username cannot be empty" -ForegroundColor Red
    exit 1
}

$repoName = "test-cpp-enterprise"
$repoUrl = "https://github.com/$username/$repoName.git"

Write-Host "`nRepository URL: $repoUrl" -ForegroundColor Green

# Initialize git if needed
if (!(Test-Path ".git")) {
    Write-Host "`nInitializing git repository..." -ForegroundColor Cyan
    git init
    Write-Host "Git initialized" -ForegroundColor Green
} else {
    Write-Host "`nGit already initialized" -ForegroundColor Green
}

# Create .gitignore if missing
if (!(Test-Path ".gitignore")) {
    Write-Host ".gitignore not found" -ForegroundColor Red
} else {
    Write-Host ".gitignore present" -ForegroundColor Green
}

# Check for uncommitted changes
Write-Host "`nChecking git status..." -ForegroundColor Cyan
$status = git status --porcelain

if ($status) {
    Write-Host "Uncommitted files found" -ForegroundColor Yellow
    
    # Add all files
    Write-Host "`nAdding all files..." -ForegroundColor Cyan
    git add .
    
    # Commit
    $commitMsg = "Initial commit: Enterprise C++ test project with CI/CD"
    Write-Host "Committing: $commitMsg" -ForegroundColor Cyan
    git commit -m $commitMsg
    
    Write-Host "Files committed" -ForegroundColor Green
} else {
    Write-Host "No uncommitted changes" -ForegroundColor Green
}

# Set main branch
Write-Host "`nSetting main branch..." -ForegroundColor Cyan
git branch -M main
Write-Host "Main branch set" -ForegroundColor Green

# Add remote
Write-Host "`nAdding remote origin..." -ForegroundColor Cyan
$existingRemote = git remote get-url origin 2>$null

if ($existingRemote) {
    Write-Host "Remote already exists: $existingRemote" -ForegroundColor Yellow
    Write-Host "   Remove it? (y/n)" -ForegroundColor Yellow
    $remove = Read-Host
    
    if ($remove -eq 'y') {
        git remote remove origin
        git remote add origin $repoUrl
        Write-Host "Remote updated" -ForegroundColor Green
    }
} else {
    git remote add origin $repoUrl
    Write-Host "Remote added" -ForegroundColor Green
}

# Instructions
Write-Host "`n" -NoNewline
Write-Host ("="*80) -ForegroundColor Cyan
Write-Host "NEXT STEPS:" -ForegroundColor Cyan
Write-Host ("="*80) -ForegroundColor Cyan

Write-Host "`n[1] Create GitHub repository:" -ForegroundColor Yellow
Write-Host "    Go to: https://github.com/new" -ForegroundColor White
Write-Host "    Repository name: $repoName" -ForegroundColor White
Write-Host "    Visibility: Public (recommended)" -ForegroundColor White
Write-Host "    Don't initialize with README (we have one)" -ForegroundColor White
Write-Host "    Click 'Create repository'" -ForegroundColor White

Write-Host "`n[2] Push code to GitHub:" -ForegroundColor Yellow
Write-Host "    git push -u origin main" -ForegroundColor Green

Write-Host "`n[3] Create GitHub Personal Access Token:" -ForegroundColor Yellow
Write-Host "    Go to: https://github.com/settings/tokens" -ForegroundColor White
Write-Host "    Click 'Generate new token (classic)'" -ForegroundColor White
Write-Host "    Name: Build Triage System" -ForegroundColor White
Write-Host "    Scopes: [X] repo, [X] workflow" -ForegroundColor White
Write-Host "    Copy the token" -ForegroundColor White

Write-Host "`n[4] Configure .env file:" -ForegroundColor Yellow
Write-Host "    Edit C:\BuildTriageSystem\.env" -ForegroundColor White
Write-Host "    Add:" -ForegroundColor White
Write-Host "    GITHUB_TOKEN=ghp_your_token_here" -ForegroundColor Green
Write-Host "    GITHUB_REPO=$username/$repoName" -ForegroundColor Green

Write-Host "`n[5] Test the triage system:" -ForegroundColor Yellow
Write-Host "    cd C:\BuildTriageSystem" -ForegroundColor Green
Write-Host "    python github_triage.py" -ForegroundColor Green

Write-Host "`n[6] Trigger a test failure:" -ForegroundColor Yellow
Write-Host "    Go to: https://github.com/$username/$repoName/actions" -ForegroundColor White
Write-Host "    Click 'Failure Scenarios'" -ForegroundColor White
Write-Host "    Run workflow -> Select 'code_error'" -ForegroundColor White

Write-Host "`n"
Write-Host ("="*80) -ForegroundColor Cyan
Write-Host "Setup helper complete!" -ForegroundColor Green
Write-Host ("="*80) -ForegroundColor Cyan

Write-Host "`nTip: Read PROJECT_COMPLETE.md for full instructions" -ForegroundColor Cyan

Write-Host "`n💡 Tip: Read PROJECT_COMPLETE.md for full instructions" -ForegroundColor Cyan
