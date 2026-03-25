public interface IMailService
{
    Task SendEmailAsync(string email, string content);
}