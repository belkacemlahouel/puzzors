# Puzzors  
  
A puzzle with lazers and mirrors.  
  
## AUTHORS  
8INF952: Maths and physics implemented in games (UQAC - University of Québec at Chicoutimi)  
  
Gabriel Rassoul - Belkacem Lahouel - Donatien Rabiller - Pierre Galmiche  
  
## HOW DOES IT WORK?  
- Unreal Engine 4.9.1  
- C++  
  
## DIRECTORIES  
TODO  
  
## TECHNICALITIES WITH GIT  
You may add an alias for this repository's address: git remote add [alias_name] [repository_address]  
  
Please configure git (at least!) such as described here:  
  
cd ~  
vim .gitconfig // or any other text editor  
  
[color]  
	diff = auto  
	status = auto  
	branch = auto  
  
[user]  
	name = [name]  
	email = [email]  
  
[alias]  
	ci = commit  
	co = checkout  
	st = status  
	br = branch  
	add-commit = !git add && git commit  
	add-ci = !git add && git commit  
  
[credential]  
	helper = cache --timeout==3600  
  
-----  
  
A .gitignore file has been added, please use it as intended.  
Read this: http://stackoverflow.com/questions/18225126/how-to-use-git-for-unity-source-control  
  
Finally, please use those commit rules, as much as possible, even though this is really annoying:  
(1) Language: english  
(2)	Title: summary of big modifications  
(3)	Skip a line  
(4)	Detail of small modifications afterwards.  
  
Please commit one big modification at a time, as much as possible.  
  
## GIT GUIDELINES  
(1) Fetch (git pull) from the Github repository.  
	* git init  
	* git remote add [alias_name] [repository_address]  
	* git pull [alias_name] master  
	* git br name.versionnumber.module // git branch  
	* git co name.versionnumber.module // git checkout  
	* Refresh project in Unity/your IDE.  
(2) Work!  
(3) Push to the Github repository once it's good!  
	* git ci -m namefile "Message." (OR) git ci -am "Message" // git commit  
	* git push [alias_name] [branch_name]  
(6) If you want to update with the Github version.  
	* git pull [alias_name] master  
  
## PUSHING RULES  
We use this model: http://nvie.com/posts/a-successful-git-branching-model/  
  
## SUMMARY  
(1) Do not push in master for no good reason.  
(2) Work on the current feature you work on, on you own branch.  
(3) Only push source code in Github (no binaries, as much as possible!).  
(4) Document your methods if needed.  
(5) Good code do not need much documentation.  
  