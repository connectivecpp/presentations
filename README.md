# Presentations and Example Code

This repository contains presentations and example code from members of the Connective C++ organization, primarily about the C++ language, but other topics and languages are covered.

Cliff Green has the following presentations, made for either the [New Mexico C++ Programmers Meetup Group](https://www.meetup.com/new-mexico-cpp-programmers/) or the [Albuquerque Google Developers Group](https://gdg.community.dev/gdg-albuquerque/):

- [A Tasty Intro to Generic Programming in C++](./presentations/ATastyIntroToGenericProgrammingInC%2B%2B.pdf)
- [Unit Testing in C++ With the Catch2 Library](https://github.com/connectivecpp/presentations/blob/main/presentations/UnitTestingC%2B%2BWithCatch2.pdf)
- [Create Your Own Open Source Project](https://github.com/connectivecpp/presentations/blob/main/presentations/CreateYourOwnOpenSourceProject.pdf)
- [std::span in C++](https://github.com/connectivecpp/presentations/blob/main/presentations/StdSpanInC%2B%2B.pdf)

Lou Langholtz is the primary organizer of the [New Mexico C++ Programmers Meetup Group](https://www.meetup.com/new-mexico-cpp-programmers/) with his own (excellent) presentations: [Lou Langholtz Presentations](https://louis-langholtz.github.io/presentation/). He also has some cool open source projects such as [PlayRho](https://louis-langholtz.github.io/PlayRho/).

As of late April 2024 the example code for all of Cliff's presentations (other than "Create Your Own Open Source Project") is present, as well as corresponding CMake files. The bulk of the presentation example code has unit tests, using the Catch2 unit testing library. The CMake files will download any needed dependencies (primarily Catch2), or optionally will work with dependency managers such as vcpkg or Conan. The [README](examples/README.md) file in the `examples` directory has additional information, including basic instructions on invoking CMake.

Documentation generation is enabled for this repository, using a GitHub Actions workflow. For more information, see the [README](examples/README.md) file in the `examples` directory.
