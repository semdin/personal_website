#pragma once

#include <string>
#include <vector>

namespace site {

/// Personal bio and profile details
struct SiteInfo {
    std::string name{"Mehmet"};
    std::string title{"Systems Architect & Full-Stack C++ Engineer"};
    std::string bio{"Passionate about low-latency systems, modern C++23, WebAssembly, and high-throughput networking. Creator of twf (The Web Framework)."};
    std::string location{"Istanbul, Turkey"};
    std::string status{"Building high-performance software"};
    std::string github_url{"https://github.com"};
};

/// Portfolio project
struct Project {
    int id{0};
    std::string title;
    std::string description;
    std::string tech_stack;
    std::string github_url;
    std::string live_url;
};

/// Skill category
struct SkillCategory {
    std::string category;
    std::vector<std::string> skills;
};

/// Contact form message sent by visitors
struct ContactMessage {
    std::string name;
    std::string email;
    std::string message;
};

/// Contact response back to the client
struct ContactResponse {
    bool success{true};
    std::string message;
};

} // namespace site
