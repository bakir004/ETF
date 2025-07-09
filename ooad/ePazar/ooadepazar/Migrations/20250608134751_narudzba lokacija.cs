using Microsoft.EntityFrameworkCore.Migrations;

#nullable disable

namespace ooadepazar.Migrations
{
    /// <inheritdoc />
    public partial class narudzbalokacija : Migration
    {
        /// <inheritdoc />
        protected override void Up(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.AddColumn<string>(
                name: "Lokacija",
                table: "Narudzba",
                type: "nvarchar(max)",
                nullable: false,
                defaultValue: "");
        }

        /// <inheritdoc />
        protected override void Down(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.DropColumn(
                name: "Lokacija",
                table: "Narudzba");
        }
    }
}
