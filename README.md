# Winjection - Windows process manipulation and code execution techniques

## Background

By using a multitude of sources, we can study, understand, and develop techniques to manipulate processes on Windows into running our code or control the execution of an already existing process.

This repository is made of primarily C++.

However, due to these techniques depending almost entirely upon the Windows API and general operating system design, many examples and their accompanying concepts are highly portable to languages with WinAPI functionality, such as Python and Go.

## Sources

Current sources include:
- Exploitation techniques
  + Spotless' ired.team
    * Code and process injection techniques: https://ired.team/offensive-security/code-injection-process-injection
  + Ashkan Hosseini of Endgame
    * 10 process injection techniques: https://www.endgame.com/blog/technical-blog/ten-process-injection-techniques-technical-survey-common-and-trending-process
- WinAPI
  + Microsoft through MSDN
    * CreateRemoteThread: https://docs.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-createremotethread
  + Raymond Chen
    * Understanding default security descriptors: https://devblogs.microsoft.com/oldnewthing/20040312-00/?p=40273
    * Blog is rich of very useful information, but many links are old and broken. Better to Google dork than click the links.

## Miscellaneous Details & Advice
- MSFVenom shellcode doesn't include a method of "returning" from itself, so by default it will not work properly in its own thread
- You can install WinDBG Preview by (https://www.thewindowsclub.com/generate-direct-download-links-for-microsoft-store-apps):
  + Grabbing the APPX (https://store.rg-adguard.net/)
  + MAYBE need to enable developer mode or sideloading apps
  + Install the APPX with Add-AppxPackage